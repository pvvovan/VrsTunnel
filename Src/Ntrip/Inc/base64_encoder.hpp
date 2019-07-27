#ifndef VRSTUNNEL_NTRIP_BASE64_ENCODER_
#define VRSTUNNEL_NTRIP_BASE64_ENCODER_

#include "login_encode.hpp"
#include <memory>

namespace VrsTunnel::Ntrip
{
    using namespace std;

    class base64_encoder : public login_encode
    {
    private:
        constexpr base64_encoder() noexcept { };
        // friend unique_ptr<lefe_encoder> make_unique<lefe_encoder>();
        friend std::unique_ptr<VrsTunnel::Ntrip::base64_encoder> std::make_unique<VrsTunnel::Ntrip::base64_encoder>();

    public:        
        string get(string name, string password) override;
        static unique_ptr<login_encode> make_instance();
        
    };
    
}

#endif /* VRSTUNNEL_NTRIP_BASE64_ENCODER_ */