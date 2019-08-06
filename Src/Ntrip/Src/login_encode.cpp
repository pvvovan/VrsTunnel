#include "base64_encoder.hpp"

namespace VrsTunnel::Ntrip
{
    std::unique_ptr<login_encode> login_encode::make_instance()
    {
        return base64_encoder::make_instance();
    }
}