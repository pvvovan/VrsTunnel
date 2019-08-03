#include <string>

#include "nmea.hpp"


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

    // NMEA GGA message generation based on provided location and time
    // speed is assumed to be 0, correction source is RTK, 12 satellies,
    // DOP 0.9, age of DGPS data record and reference station ID are 0,
    // both heights are equal to elevetion
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
        int centiseconds = milliseconds.count() / 10;

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

        int print_res = snprintf(buf, bsize, "GPGGA,%02ld%02ld%02ld.%02d,%02d%011.8f,%s,%03d%011.8f,%s,4,12,0.9,%.3f,M,%.3f,M,0,0000", 
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