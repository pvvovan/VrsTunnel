#include <string>
#include <cstdio>

#include <nmea.hpp>


namespace VrsTunnel::Ntrip
{
    std::string nmea::getChecksum(std::string data) {
        const char* str = data.c_str();
        int len = data.length();
        uint16_t checksum = *str++;
        for(int i = 1; i < len; ++i) {
            checksum ^= *str++;
        }
        char cs[] = {"00"};
        sprintf(cs, "%02X", checksum);
        return cs;
    }
}