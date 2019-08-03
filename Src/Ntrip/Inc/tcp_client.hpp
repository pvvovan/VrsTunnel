#ifndef TCP_ASYNCHRONOUS_CLIENT_
#define TCP_ASYNCHRONOUS_CLIENT_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "async_io.hpp"

#include <iostream>

namespace VrsTunnel::Ntrip
{
    class tcp_client
    {
    private:
        int m_sockfd{-1};

    public:
        tcp_client() = default;

        tcp_client(const tcp_client&)               = delete;
        tcp_client& operator=(const tcp_client&)    = delete;
        tcp_client(tcp_client&&)                    = delete;
        tcp_client& operator=(tcp_client&&)         = delete;

        ~tcp_client();
        void Close();
        [[nodiscard]] io_status Connect(std::string addr, int port);
        int get_sockfd() noexcept;
    };
}

#endif /* TCP_ASYNCHRONOUS_CLIENT_ */