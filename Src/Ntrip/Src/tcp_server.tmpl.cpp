#include <string>

#include "tcp_server.hpp.cpp"
#include "accept_listener.hpp"

namespace VrsTunnel::Ntrip
{
    template bool tcp_server::start(int, accept_listener&);
    template void tcp_server::run_accepting(struct sockaddr*, int, accept_listener&);
}