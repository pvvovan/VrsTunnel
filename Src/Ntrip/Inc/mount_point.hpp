#ifndef VRSTUNNEL_NTRIP_MOUNT_POINT_
#define VRSTUNNEL_NTRIP_MOUNT_POINT_

#include <string>
#include <vector>

#include "location.hpp"

namespace VrsTunnel::Ntrip
{
    /**
     * NTRIP mount point struct with static parsing method
     */
    struct mount_point
    {
        std::string raw_entry;    /**< Raw table line from NTRIP Caster */
        location reference; /**< Mount point position coordinates */
        std::string name;   /**< Mount point name to be show */
        std::string type;   /**< GNSS RTK correction type */

        mount_point(std::string_view line);
        ~mount_point() = default;
        mount_point(const mount_point&) = default;
        mount_point(mount_point&&) = default;
        mount_point& operator=(const mount_point&) = default;
        mount_point& operator=(mount_point&&) = default;

        private:
        static std::string parse_name(std::string_view line); /**< Retrieves name from NTRIP mount point table entry */
        static std::string parse_type(std::string_view line); /**< Retrieves GNSS correction type from NTRIP mount point table entry */
        static location parse_reference(std::string_view line); /**< Retrieves location of mount point */
        static std::string_view get_item(std::string_view line, int index);
        
        /**
         * Helper method to parse NTRIP mount point table
         */
        public:
        static std::vector<mount_point> parse_table(std::string_view data);
    };

}

#endif /* VRSTUNNEL_NTRIP_MOUNT_POINT_ */