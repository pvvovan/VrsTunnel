#include <functional>
#include <charconv>
#include <cmath>
#include <memory>

#include "NtripClient.hpp"
#include "base64_encoder.hpp"

namespace VrsTunnel::Ntrip
{
    std::variant<std::vector<MountPoint>, io_status>
    NtripClient::getMountPoints(std::string address, int tcpPort, 
            std::string name, std::string password)
    {
        tcp_client tc{};
        tc.Connect(address, tcpPort);
        async_io aio{tc.get_sockfd()};
        const char* requestFormat = "GET / HTTP/1.0\r\n"
            "User-Agent: NTRIP PvvovanNTRIPClient/\r\n"
            "Accept: */*\r\n" "Connection: close\r\n" 
            "Authorization: Basic %s\r\n" "\r\n";
        
        std::unique_ptr<char[]> request;
        std::string auth{""};
        if (name.size() > 0) {
            std::unique_ptr<login_encode> encoder = base64_encoder::make_instance();
            auth = (*encoder).get(name, password);
        }
        request = std::make_unique<char[]>(strlen(requestFormat) + auth.length());
        sprintf(request.get(), requestFormat, auth.c_str());
        
        
        auto res = aio.Write(request.get(), strlen(request.get()));
        if (res != io_status::Success) {
            return res;
        }

        std::string responseRaw{};
        for(int i = 0; i < 50; i++) { // 5 seconds timeout
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto avail = aio.Available();
            if (avail < 0) {
                return io_status::Error;
            }
            else if (avail > 0) {
                auto chunk = aio.Read(avail);
                responseRaw.append(chunk.get(), avail);
                if (this->hasTableEnding(responseRaw)) {
                    break;
                }
            }
        }

        if (!this->hasTableEnding(responseRaw)) {
            return io_status::Error;
        }

        return parseTable(responseRaw);
    }

    bool NtripClient::hasTableEnding(std::string_view data)
    {
        const std::string tableEnding {"ENDSOURCETABLE\r\n"};
        if (data.length() >= tableEnding.length()) {
            return (data.compare(data.length() - tableEnding.length(),
                tableEnding.length(), tableEnding) == 0);
        }
        else {
            return false;
        }
    }

    std::vector<MountPoint> NtripClient::parseTable(std::string_view data)
    {
        auto mountPoints = std::vector<MountPoint>();
        std::size_t tableStart = data.find("\r\n\r\n");
        if (tableStart != std::string::npos) {
            std::size_t rowStart = tableStart + 4;
            std::size_t rowEnd = data.find("\r\n", rowStart);
            while (rowEnd != std::string::npos) {
                MountPoint mp{};
                mp.Raw = data.substr(rowStart, rowEnd - rowStart);
                if (mp.Raw != "ENDSOURCETABLE") {
                    mp.Name = getName(mp.Raw);
                    mp.Reference = getReference(mp.Raw);
                    mountPoints.emplace_back(mp);
                }
                rowStart = rowEnd + 2;
                rowEnd = data.find("\r\n", rowStart);
            }
        }
        return mountPoints;
    }

    std::string NtripClient::getName(std::string_view line)
    {
        std::size_t nameStart = line.find(";");
        if (nameStart != std::string::npos) {
            ++nameStart;
            std::size_t nameStop = line.find(";", nameStart);
            if (nameStop != std::string::npos) {
                return std::string(line.substr(nameStart, nameStop - nameStart));
            }
        }
        return "";
    }

    location NtripClient::getReference(std::string_view line)
    {
        std::function<std::size_t(std::string_view, std::size_t, std::string_view, std::size_t)> find_Nth;

        find_Nth = [&find_Nth]
            (std::string_view haystack, std::size_t pos, std::string_view needle, std::size_t nth)
            -> std::size_t 
        {
            std::size_t found_pos = haystack.find(needle, pos);
            if (nth == 0 || std::string::npos == found_pos) {
                return found_pos;
            }
            return find_Nth(haystack, found_pos + 1, needle, nth - 1);
        };

        location loc;
        auto parse = [&line, &find_Nth] (int pos) -> double {
            std::size_t start = find_Nth(line, 0, ";", pos)+1;
            std::size_t stop = find_Nth(line, 0, ";", pos + 1);
            std::string_view sv_float = line.substr(start, stop - start);
            sv_float.data();
            std::size_t dotPos = sv_float.find(".");
            if (dotPos != std::string::npos) {
                int integ = 0, frac = 0;
                std::string_view sv_integ = sv_float.substr(0, dotPos);
                std::from_chars(sv_integ.data(), sv_integ.data() + sv_integ.size(), integ);
                std::string_view sv_frac = sv_float.substr(dotPos + 1);
                std::from_chars(sv_frac.data(), sv_frac.data() + sv_frac.size(), frac);
                double value = 0;
                value = (static_cast<double>(frac)) / (std::pow(10, sv_frac.size())) + integ;
                return value;
            }
            else {
                int value = 0;
                std::from_chars(sv_float.data(), sv_float.data() + sv_float.size(), value);
                return value;
            }
        };

        loc.Latitude = parse(8);
        loc.Longitude = parse(9);
        return loc;
    }
}