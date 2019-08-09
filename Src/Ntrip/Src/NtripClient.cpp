#include <functional>
#include <charconv>
#include <cmath>
#include <memory>

#include "NtripClient.hpp"
#include "login_encode.hpp"
#include "nmea.hpp"

namespace VrsTunnel::Ntrip
{
    std::unique_ptr<char[]> NtripClient::build_request(const char* mountpoint,
            std::string name, std::string password)
    {
        const char* requestFormat = "GET /%s HTTP/1.0\r\n"
            "User-Agent: NTRIP PvvovanNTRIPClient/1.0.0\r\n"
            "Accept: */*\r\n" "Connection: close\r\n" 
            "Authorization: Basic %s\r\n" "\r\n";
        
        std::unique_ptr<char[]> request;
        std::string auth{""};
        if (name.size() > 0) {
            std::unique_ptr<login_encode> encoder = login_encode::make_instance();
            auth = (*encoder).get(name, password);
        }
        request = std::make_unique<char[]>(strlen(requestFormat) + strlen(mountpoint) + auth.length() + 2);
        sprintf(request.get(), requestFormat, mountpoint, auth.c_str());
        return request;
    }

    std::variant<std::vector<MountPoint>, io_status>
    NtripClient::getMountPoints(std::string address, int tcpPort, 
            std::string name, std::string password)
    {
        tcp_client tc{};
        auto con_res = tc.connect(address, tcpPort);
        if (con_res != io_status::Success) {
            return con_res;
        }

        async_io aio{tc.get_sockfd()};
        std::unique_ptr<char[]> request = build_request("", name, password);
        
        auto res = aio.write(request.get(), strlen(request.get()));
        if (res != io_status::Success) {
            return res;
        }

        std::string responseRaw{};
        for(int i = 0; i < 50; i++) { // 5 seconds timeout
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto avail = aio.available();
            if (avail < 0) {
                return io_status::Error;
            }
            else if (avail > 0) {
                auto chunk = aio.read(avail);
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
                    mp.Type = getType(mp.Raw);
                    mp.Reference = getReference(mp.Raw);
                    mountPoints.emplace_back(std::move(mp));
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

    std::string NtripClient::getType(std::string_view line)
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

        std::size_t start = find_Nth(line, 0, ";", 2);
        if (start == std::string::npos) {
            return std::string();
        }
        start++;
        std::size_t stop = find_Nth(line, 0, ";", 3);
        if (stop == std::string::npos) {
            return std::string();
        }
        std::string_view sv_type = line.substr(start, stop - start);
        return std::string(sv_type);
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

    status NtripClient::connect(ntrip_login nlogin)
    {
        if (m_tcp) {
            throw std::runtime_error("tcp connection already created");
        }
        m_tcp = std::make_unique<tcp_client>();
        auto con_res = m_tcp->connect(nlogin.address, nlogin.port);
        if (con_res != io_status::Success) {
            m_tcp.reset();
            return status::error;
        }
        m_aio = std::make_unique<async_io>(m_tcp->get_sockfd());
        std::unique_ptr<char[]> request = build_request(nlogin.mountpoint.data(), nlogin.username, nlogin.password);
        auto res = m_aio->write(request.get(), strlen(request.get()));
        if (res != io_status::Success) {
            m_status = status::error;
            return m_status;
        }

        // read authentication result
        std::string responseText{};
        for(int i = 1; i < 50; ++i) { // 5 second timeout
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto avail = m_aio->available();
            if (avail < 0) {
                return status::error;
            }
            else if (avail > 0) {
                auto chunk = m_aio->read(avail);
                responseText.append(chunk.get(), avail);
                const std::string ending {"\r\n\r\n"};
                if (responseText.length() >= ending.length()) {
                    if (responseText.compare(responseText.length() - ending.length(),
                                ending.length(), ending) == 0) {
                        break;
                    }
                }
            }
        }

        auto startsWith = [text = &responseText](std::string_view start) -> bool
        {
            if (start.size() > text->size()) {
                return false;
            }
            return text->compare(0, start.size(), start) == 0;
        };

        if (startsWith("ICY 200 OK\r\n")) {
            m_status = status::ready;
        }
        else if (startsWith("HTTP/1.1 401 Unauthorized\r\n")) {
            m_status = status::authfailure;
        } 
        else if (startsWith("HTTP/1.1 404 Not Found\r\n")) {
            m_status = status::nomount;
        } 
        else {
            m_status = status::error;
        }
        return m_status;
    }

    int NtripClient::available()
    {
        return m_aio->available();
    }

    std::unique_ptr<char[]> NtripClient::receive(int size)
    {
        return m_aio->read(size);
    }

    [[nodiscard]] io_status NtripClient::send_gga(location location, std::chrono::system_clock::time_point time)
    {
        if (!m_aio) {
            throw std::runtime_error("no tcp connection");
        }
        std::string gga = std::get<std::string>(nmea::getGGA(location, time));
        return m_aio->write(gga.c_str(), gga.length());
    }

    status NtripClient::get_status()
    {
        if (m_status == status::ready) {
            io_status res = m_aio->check();
            switch (res)
            {
            case io_status::Success:
                m_status = status::ready;
                break;
            case io_status::InProgress:
                m_status = status::sending;
                break;
            
            default:
                m_status = status::error;
                break;
            }
        }
        return m_status;
    }

    void NtripClient::disconnect()
    {
        while (m_aio->check() == io_status::InProgress) { } // timeout missing?
        m_aio->end();
        m_tcp->close();
        m_status = status::uninitialized;
    }

    ssize_t NtripClient::send_end()
    {
        return m_aio->end();
    }
}