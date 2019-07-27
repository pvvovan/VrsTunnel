#ifndef VRSTUNNEL_NTRIP_NMEA_
#define VRSTUNNEL_NTRIP_NMEA_

#include <string>
#include <charconv>

#include "location.hpp"

namespace VrsTunnel::Ntrip
{
    class Nmea
    {
    private:
        constexpr Nmea() noexcept { };
    
    public:
    static std::string getGGA(location location) {
        std::string str { "xxxxxxxx" };
        const int value = 1986;
        const auto res = std::to_chars(str.data(),
            str.data() + str.size(),
            value);
        return "$GPGGA,";
    }

    static std::string getChecksum(std::string data);
    };    
}
#endif