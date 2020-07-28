#include <bits/c++config.h>
#include <bits/stdint-uintn.h>
#include <string>
#include <cinttypes>

#include "nmea.hpp"


namespace VrsTunnel::Ntrip
{
    uint8_t nmea::checksum(std::string_view data) {
        std::size_t len = data.length();
        uint8_t checksum = static_cast<uint8_t>(data[0]);
        for(std::size_t i = 1; i < len; ++i) {
            checksum = static_cast<uint8_t>(checksum ^ data[i]);
        }
        return checksum;
    }

    [[nodiscard]] std::variant<std::string, nmea::ErrorCode>
    nmea::getGGA(location location, std::chrono::system_clock::time_point time) {
        constexpr int bsize = 256;
        char buf[bsize] = { 0 };
        auto span = time.time_since_epoch();

        using namespace std;
        using chrono_days = chrono::duration<long, std::ratio<86400>>;
        chrono_days days = chrono::duration_cast<chrono_days>(span);
        chrono::hours hours = chrono::duration_cast<chrono::hours>(span -= days);
        chrono::minutes minutes = chrono::duration_cast<chrono::minutes>(span -= hours);
        chrono::seconds seconds = chrono::duration_cast<chrono::seconds>(span -= minutes);
        chrono::milliseconds milliseconds = chrono::duration_cast<chrono::milliseconds>(span -= seconds);
        int centiseconds = static_cast<int>(milliseconds.count() / 10);

        const char* NS = "N";
        if (location.Latitude < 0) {
            NS = "S";
            location.Latitude = -location.Latitude;
        }
        const char* EW = "E";
        if (location.Longitude < 0) {
            EW = "W";
            location.Longitude = -location.Longitude;
        }

        int print_res = snprintf(buf, bsize, "GPGGA,%02" PRId64 "%02" PRId64 "%02" PRId64 ".%02d,%02d%011.8f,%s,%03d%011.8f,%s,4,12,0.9,%.3f,M,%.3f,M,0,0000", 
            hours.count(), minutes.count(), seconds.count(), centiseconds,
            static_cast<int>(location.Latitude), (location.Latitude - static_cast<int>(location.Latitude))*60, NS,
            static_cast<int>(location.Longitude), (location.Longitude - static_cast<int>(location.Longitude))*60, EW,
            location.Elevation, location.Elevation);

        if(print_res < 0) {
            return nmea::ErrorCode::Undefined;
        }

        char gga[bsize] = { 0 };
        print_res = snprintf(gga, bsize, "$%s*%X\r\n", buf, nmea::checksum(std::string{buf}));
        if(print_res < 0) {
            return nmea::ErrorCode::Undefined;
        }

        return gga;
    }
}