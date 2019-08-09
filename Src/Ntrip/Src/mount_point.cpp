#include <functional>
#include <charconv>
#include <cmath>

#include "mount_point.hpp"

namespace VrsTunnel::Ntrip
{
    std::vector<mount_point> mount_point::parse_table(std::string_view data)
    {
        auto mountPoints = std::vector<mount_point>();
        std::size_t tableStart = data.find("\r\n\r\n");
        if (tableStart != std::string::npos) {
            std::size_t rowStart = tableStart + 4;
            std::size_t rowEnd = data.find("\r\n", rowStart);
            while (rowEnd != std::string::npos) {
                mount_point mp{};
                mp.Raw = data.substr(rowStart, rowEnd - rowStart);
                if (mp.Raw != "ENDSOURCETABLE") {
                    mp.Name = parse_name(mp.Raw);
                    mp.Type = parse_type(mp.Raw);
                    mp.Reference = parse_reference(mp.Raw);
                    mountPoints.emplace_back(std::move(mp));
                }
                rowStart = rowEnd + 2;
                rowEnd = data.find("\r\n", rowStart);
            }
        }
        return mountPoints;
    }

    std::string mount_point::parse_name(std::string_view line)
    {
        return std::string(get_item(line, 0));
    }

    std::string mount_point::parse_type(std::string_view line)
    {
        return std::string(get_item(line, 2));
    }

    location mount_point::parse_reference(std::string_view line)
    {
        auto parse = [&line] (int pos) -> double {
            auto sv_float = get_item(line, pos);
            std::size_t dotPos = sv_float.find(".");
            if (dotPos != std::string::npos) {
                int integ = 0, frac = 0;
                std::string_view sv_integ = sv_float.substr(0, dotPos);
                std::from_chars(sv_integ.data(), sv_integ.data() + sv_integ.size(), integ);
                std::string_view sv_frac = sv_float.substr(dotPos + 1);
                std::from_chars(sv_frac.data(), sv_frac.data() + sv_frac.size(), frac);
                double value = 0;
                value = (static_cast<double>(frac)) / (std::pow(10, sv_frac.size())) + integ;
                return value;
            }
            else {
                int value = 0;
                std::from_chars(sv_float.data(), sv_float.data() + sv_float.size(), value);
                return value;
            }
        };

        location loc{};
        loc.Latitude = parse(8);
        loc.Longitude = parse(9);
        return loc;
    }

    std::string_view mount_point::get_item(std::string_view line, int index)
    {
        using namespace std;
        function<std::size_t(string_view, std::size_t, string_view, std::size_t)> find_Nth;

        find_Nth = [&find_Nth]
            (string_view haystack, std::size_t pos, string_view needle, std::size_t nth)
                -> std::size_t 
        {
            std::size_t found_pos = haystack.find(needle, pos);
            if (nth == 0 || std::string::npos == found_pos) {
                return found_pos;
            }
            return find_Nth(haystack, found_pos + 1, needle, nth - 1);
        };

        std::size_t start = find_Nth(line, 0, ";", index);
        if (start == std::string::npos) {
            return std::string_view();
        }
        start++;
        std::size_t stop = find_Nth(line, start, ";", 0);
        if (stop == std::string::npos) {
            return std::string_view();
        }
        return line.substr(start, stop - start);
    }
}