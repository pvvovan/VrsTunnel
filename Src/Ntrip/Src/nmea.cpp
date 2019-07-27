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

    std::string nmea::getGGA(location location, std::chrono::system_clock::time_point time) {
        char buffer[256] = { 0 };
        auto duration = time.time_since_epoch();
        std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(duration);
        std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
        std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);

        const char* N = "N";
        if (location.Latitude < 0) {
            N = "S";
            location.Latitude = -location.Latitude;
        }
        const char* E = "E";
        if (location.Longitude < 0) {
            E = "W";
            location.Longitude = -location.Longitude;
        }

        sprintf(buffer, "$GPGGA,%02ld%02ld%02ld.00,%02d%09.6f,%s,%03d%09.6f,%s,", 
            hours.count(), minutes.count(), seconds.count(),
            static_cast<int>(location.Latitude), (location.Latitude - static_cast<int>(location.Latitude))*60, N,
            static_cast<int>(location.Longitude), (location.Longitude - static_cast<int>(location.Longitude))*60, E);

        std::string str { "xxxxxxxx" };
        const int value = 1986;
        const auto res = std::to_chars(str.data(),
            str.data() + str.size(),
            value);
        return buffer;
    }
}