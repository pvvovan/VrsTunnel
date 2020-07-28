#include <string>

#include "tcp_server.hpp.cpp"
#include "accept_listener.hpp"

namespace VrsTunnel::Ntrip
{
    template bool tcp_server::start(uint16_t port, accept_listener& listener);
    template void tcp_server::run_accepting(struct sockaddr* addr, int sockfd, accept_listener& listener);
}