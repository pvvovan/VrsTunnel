#ifndef __VRSTUNNEL_NTRIP_LOGIN_ENCODE_
#define __VRSTUNNEL_NTRIP_LOGIN_ENCODE_

#include <string>

namespace VrsTunnel::Ntrip
{
    using std::string;

    class login_encode
    {
    private:
        /* data */
    public:
        login_encode() = default;
        virtual ~login_encode() = default;
        virtual string get(string name, string password) = 0;
    };
    
    
    
}

#endif /* __VRSTUNNEL_NTRIP_LOGIN_ENCODE_ */