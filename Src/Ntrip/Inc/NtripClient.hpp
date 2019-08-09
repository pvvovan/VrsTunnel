#ifndef VRSTUNNEL_NTRIP_NTRIP_CLIENT_
#define VRSTUNNEL_NTRIP_NTRIP_CLIENT_

#include <string>
#include <vector>
#include <thread>
#include <variant>
#include <chrono>

#include "location.hpp"
#include "tcp_client.hpp"
#include "async_io.hpp"
#include "ntrip_login.hpp"
#include "tcp_client.hpp"

namespace VrsTunnel::Ntrip
{
    /**
     * NTRIP mount point
     */
    struct MountPoint
    {
        std::string Raw;    /**< Raw data from NTRIP caster */
        location Reference; /**< Mount point position coordinates */
        std::string Name;   /**< Mount point name to be show */
        std::string Type;   /**< GNSS RTK correction type */
    };

    /**
     * NTRIP client status
     */
    enum class status { uninitialized, ready, error, authfailure, nomount, sending };
    
    /**
     * NTRIP client class.
     * The main task is to provide RTK correction from NTRIP Caster to GNSS receiver.
     */
    class NtripClient
    {
    private:
        std::string getName(std::string_view line); /**< Retrieves name from NTRIP mount point table entry */
        std::string getType(std::string_view line); /**< Retrieves GNSS correction type from NTRIP mount point table entry */
        location getReference(std::string_view line); /**< Retrieves location of mount point */
        std::unique_ptr<async_io> m_aio {nullptr}; /**< Asyncronous operations */
        std::unique_ptr<tcp_client> m_tcp {nullptr}; /**< TCP connection */
        status m_status {status::uninitialized}; /**< Current status of the client */

        std::unique_ptr<char[]> build_request(const char* mountpoint,
                std::string name, std::string password);
        
        NtripClient(const NtripClient&) = delete;               /**< No copy constructor */
        NtripClient(NtripClient&&) = delete;                    /**< No move costructor */
        NtripClient& operator=(const NtripClient&) = delete;    /**< No copy operator */
        NtripClient& operator=(NtripClient&&) = delete;         /**< No move operator */

    public:
        NtripClient() = default;
        ~NtripClient() = default;

        /**
         * Download mount point table
         */
        std::variant<std::vector<MountPoint>, io_status>
        getMountPoints(std::string address, int tcpPort, 
            std::string name = std::string(), std::string password = std::string());

        /**
         * Helper method to check if download is complete
         */
        bool hasTableEnding(std::string_view data);

        /**
         * Helper method to parse NTRIP mount point table
         */
        std::vector<MountPoint> parseTable(std::string_view data);

        /**
         * Create connection with NTRIP Caster
         * @param nlogin login information
         * @return result of the connection
         */
        [[nodiscard]] status connect(ntrip_login nlogin);

        /**
         * Disconnect from NTRIP Caster
         */
        void disconnect();

        /**
         * @return amount of available RTK correction
         */
        int available();

        /**
         * Get available RTK correction
         * @param size amount to receive
         * @return RTK correction data
         */
        std::unique_ptr<char[]> receive(int size);

        /**
         * Provides NMEA GGA message to NTRIP Caster.
         * @param location coordinates of NTRIP Client position
         * @param time epoch when position was measured
         * @return status of the transmission request
         */
        [[nodiscard]] io_status send_gga(location location, std::chrono::system_clock::time_point time);

        /**
         * @return status associated with AIOCBP.
         */
        ssize_t send_end();

        /**
         * @return current status of the connection
         */
        status get_status();
    };

}

#endif