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
#include "ntrip_login.hpp"
#include "tcp_client.hpp"

namespace VrsTunnel::Ntrip
{
    struct MountPoint
    {
        std::string Raw;
        location Reference;
        std::string Name;
    };

    enum class status { uninitialized, ready, error, authfailure, nomount, sending };
    
    class NtripClient
    {
    private:
        std::string getName(std::string_view line);
        location getReference(std::string_view line);
        std::unique_ptr<async_io> m_aio {nullptr};
        std::unique_ptr<tcp_client> m_tcp {nullptr};
        status m_status {status::uninitialized};

        std::unique_ptr<char[]> build_request(const char* mountpoint,
                std::string name, std::string password);

    public:
        NtripClient() = default;
        ~NtripClient() = default;

        std::variant<std::vector<MountPoint>, io_status>
        getMountPoints(std::string address, int tcpPort, 
            std::string name = std::string(), std::string password = std::string());

        bool hasTableEnding(std::string_view data);

        std::vector<MountPoint> parseTable(std::string_view data);

        [[nodiscard]] status connect(ntrip_login);
        void disconnect();
        int available();
        std::unique_ptr<char[]> receive(int size);
        [[nodiscard]] io_status send_gga(location location, std::chrono::system_clock::time_point time);
        void send_end();
        status get_status();
    };

}

#endif