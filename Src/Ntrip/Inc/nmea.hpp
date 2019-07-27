#ifndef VRSTUNNEL_NTRIP_NMEA_
#define VRSTUNNEL_NTRIP_NMEA_

#include <string>
#include <charconv>
#include <chrono>

#include "location.hpp"

namespace VrsTunnel::Ntrip
{
    class nmea
    {
    private:
        constexpr nmea() noexcept { };
    
    public:
    static std::string getGGA(location location, std::chrono::system_clock::time_point time);
    static uint8_t checksum(std::string_view data);
    
    };    
}
#endif