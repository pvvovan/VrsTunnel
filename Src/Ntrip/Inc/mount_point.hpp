#ifndef VRSTUNNEL_NTRIP_MOUNT_POINT_
#define VRSTUNNEL_NTRIP_MOUNT_POINT_

#include <string>
#include <vector>

#include "location.hpp"

namespace VrsTunnel::Ntrip
{
    /**
     * NTRIP mount point struct with static parsing methods
     */
    struct mount_point
    {
        std::string Raw;    /**< Raw data from NTRIP caster */
        location Reference; /**< Mount point position coordinates */
        std::string Name;   /**< Mount point name to be show */
        std::string Type;   /**< GNSS RTK correction type */

        static std::string parse_name(std::string_view line); /**< Retrieves name from NTRIP mount point table entry */
        static std::string parse_type(std::string_view line); /**< Retrieves GNSS correction type from NTRIP mount point table entry */
        static location parse_reference(std::string_view line); /**< Retrieves location of mount point */

        /**
         * Helper method to parse NTRIP mount point table
         */
        static std::vector<mount_point> parse_table(std::string_view data);

        private:
        static std::string_view get_item(std::string_view line, int index);
    };

}

#endif /* VRSTUNNEL_NTRIP_MOUNT_POINT_ */