#include <string>

#include <nmea.hpp>


namespace VrsTunnel::Ntrip
{
    uint8_t nmea::checksum(std::string_view data) {
        int len = data.length();
        uint8_t checksum = data[0];
        for(int i = 1; i < len; ++i) {
            checksum ^= data[i];
        }
        return checksum;
    }
}