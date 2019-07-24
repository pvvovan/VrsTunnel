#ifndef _VRSTUNNEL_NTRIP_LEFE_ENCODER_
#define _VRSTUNNEL_NTRIP_LEFE_ENCODER_

#include "login_encode.hpp"
#include <memory>

namespace VrsTunnel::Ntrip
{
    using namespace std;

    class lefe_encoder : public login_encode
    {
    private:
        lefe_encoder() = default;

    public:        
        string get(string name, string password) override;
        static unique_ptr<lefe_encoder> make();
        friend unique_ptr<lefe_encoder> make_unique<lefe_encoder>();
    };
    
}

#endif /* _VRSTUNNEL_NTRIP_LEFE_ENCODER_ */