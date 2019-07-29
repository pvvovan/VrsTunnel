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
        io_status Connect(std::string addr, int port);
        int get_sockfd();
    };
    
    tcp_client::~tcp_client()
    {
        Close();
    }
    
    io_status tcp_client::Connect(std::string address, int port)
    {
        struct addrinfo hints = { 0 };
        struct addrinfo *result, *rp;
        int sfd;

        /* Obtain address(es) matching host/port */
        // memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family     = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype   = SOCK_STREAM;  /* TCP socket */
        hints.ai_flags      = 0;
        hints.ai_protocol   = 0;            /* Any protocol */

        constexpr int psize = 10;
        char port_c[psize + 1] = { 0 };
        snprintf(port_c, psize, "%d", port);
        int s = getaddrinfo(address.c_str(), port_c, &hints, &result);
        if (s != 0) {
            return io_status::Error;
        }

        /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully connect(2).
        If socket(2) (or connect(2)) fails, we (close the socket
        and) try the next address. */
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            sfd = socket(rp->ai_family, rp->ai_socktype,
                            rp->ai_protocol);
            if (sfd == -1)
                continue;

            if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
                break;                  /* Success */
            }

            close(sfd);
        }

        if (rp == NULL) {               /* No address succeeded */
            return io_status::Error;
        }
        freeaddrinfo(result);           /* No longer needed */

        m_sockfd = sfd;
        return io_status::Success;
    }

    void tcp_client::Close()
    {
        if (m_sockfd != -1) {
            close(m_sockfd);
        }
    }

    int tcp_client::get_sockfd()
    {
        return this->m_sockfd;
    }
}

#endif /* TCP_ASYNCHRONOUS_CLIENT_ */