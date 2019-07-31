#ifndef VRSTUNNEL_NTRIP_NTRIP_CLIENT_
#define VRSTUNNEL_NTRIP_NTRIP_CLIENT_

#include <string>
#include <vector>
#include <thread>
#include <variant>
#include <chrono>

#include "location.hpp"
#include "tcp_client.hpp"
#include "async_io.hpp"

namespace VrsTunnel::Ntrip
{
    struct MountPoint
    {
        std::string Raw;
        location Reference;
        std::string Name;
    };
    
    class NtripClient
    {
    private:
        /* data */
    public:
        NtripClient() = default;
        ~NtripClient() = default;

        std::variant<std::vector<MountPoint>, io_status>
        getMountPoints(std::string address, int tcpPort);
        bool hasTableEnding(std::string rawTable);
    };

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
                if (hasTableEnding(responseRaw)) {
                    break;
                }
            }
        }
        if (!hasTableEnding(responseRaw)) {
            return io_status::Error;
        }
        MountPoint mp{};
        mp.Raw = std::move(responseRaw);
        std::vector<MountPoint> mps { mp };
        return mps;
    }

    bool NtripClient::hasTableEnding(std::string rawTable)
    {
        std::string tableEnding {"ENDSOURCETABLE\r\n"};
        if (rawTable.length() >= tableEnding.length()) {
            return (0 == rawTable.compare (rawTable.length() - tableEnding.length(), 
                tableEnding.length(), tableEnding));
        } 
        else {
            return false;
        }
    }
}

#endif