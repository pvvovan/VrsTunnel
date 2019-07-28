#ifndef VRSTUNNEL_NTRIP_NMEA_
#define VRSTUNNEL_NTRIP_NMEA_

#include <string>
#include <charconv>
#include <chrono>
#include <variant>

#include "location.hpp"

namespace VrsTunnel::Ntrip
{
    class nmea
    {
    private:
        constexpr nmea() noexcept { };
    
    public:
    enum class [[nodiscard]] ErrorCode
    {
        Undefined
    };

    [[nodiscard]] static std::variant<std::string, nmea::ErrorCode>
    getGGA(location location, std::chrono::system_clock::time_point time);
    static uint8_t checksum(std::string_view data);
    
    };    
}
#endif