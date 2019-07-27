#include <string>

#include <nmea.hpp>

namespace VrsTunnel::Ntrip
{
    std::string Nmea::getChecksum(std::string data) {
        return "11";
    }
}