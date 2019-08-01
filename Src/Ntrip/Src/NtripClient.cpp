#include "NtripClient.hpp"

namespace VrsTunnel::Ntrip
{
    std::variant<std::vector<MountPoint>, io_status>
    NtripClient::getMountPoints(std::string address, int tcpPort)
    {
        tcp_client tc{};
        tc.Connect(address, tcpPort);
        async_io aio{tc.get_sockfd()};
        const char* request = "GET / HTTP/1.0\r\n"
            "User-Agent: NTRIP PvvovanNTRIPClient/\r\n"
            "Accept: */*\r\n" "Connection: close\r\n" "\r\n";
        
        auto res = aio.Write(request, strlen(request));
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
        MountPoint mp{};
        mp.Raw = std::move(responseRaw);
        std::vector<MountPoint> mps { mp };
        return mps;
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
        std::size_t rowStart = tableStart + 4;
        if (tableStart != std::string::npos) {
            std::size_t rowEnd = data.find("\r\n", rowStart);
            while (rowEnd != std::string::npos) {
                MountPoint mp{};
                mp.Raw = data.substr(rowStart, rowEnd - rowStart);
                if (mp.Raw != "ENDSOURCETABLE") {
                    mountPoints.emplace_back(mp);
                }
                rowStart = rowEnd + 2;
                rowEnd = data.find("\r\n", rowStart);
            }
        }
        return mountPoints;
    }
}