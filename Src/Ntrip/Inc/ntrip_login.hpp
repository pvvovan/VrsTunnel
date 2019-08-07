#ifndef VRSTUNNEL_NTRIP_NTRIP_LOGIN_
#define VRSTUNNEL_NTRIP_NTRIP_LOGIN_

#include <string>

#include "location.hpp"

namespace VrsTunnel::Ntrip
{
    struct ntrip_login
    {
        std::string address;
        int port;
        std::string username;
        std::string password;
        std::string mountpoint;
        location position;
    };
    
}

#endif /* VRSTUNNEL_NTRIP_NTRIP_LOGIN_ */