#include "tcp_client.hpp"

namespace VrsTunnel::Ntrip
{
    tcp_client::~tcp_client()
    {
        close();
    }

    tcp_client::tcp_client(int fd) :
        m_sockfd{fd}
    { }
    
    [[nodiscard]] io_status tcp_client::connect(std::string address, int port)
    {
        struct addrinfo hints;
        ::memset(&hints, 0, sizeof(hints));
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

            if (::connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
                break;                  /* Success */
            }

            ::close(sfd);
        }

        if (rp == NULL) {               /* No address succeeded */
            return io_status::Error;
        }
        freeaddrinfo(result);           /* No longer needed */

        m_sockfd = sfd;
        return io_status::Success;
    }

    void tcp_client::close()
    {
        if (m_sockfd != -1) {
            int res = ::close(m_sockfd);
            if (res != 0) {
                throw std::runtime_error("possible memory leak");
            }
            m_sockfd = -1;
        }
    }

    int tcp_client::get_sockfd() noexcept
    {
        return this->m_sockfd;
    }
}