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
#include "mount_point.hpp"

namespace VrsTunnel::Ntrip
{
    /**
     * NTRIP client status
     */
    enum class status { uninitialized, ready, error, authfailure, nomount, sending };
    
    /**
     * NTRIP client class.
     * The main task is to provide RTK correction from NTRIP Caster to GNSS receiver.
     */
    class ntrip_client
    {
    private:
        std::unique_ptr<async_io> m_aio {nullptr};      /**< Asyncronous operations */
        std::unique_ptr<tcp_client> m_tcp {nullptr};    /**< TCP connection */
        status m_status {status::uninitialized};        /**< Current status of the client */

        /**
         * Build HTTP GET request
         * @param mountpoint name of NTRIP mount point
         * @param name NTRIP user name
         * @param passwword NTRIP user password
         * @return HTTP GET request buffer (null terminated)
         */
        std::unique_ptr<char[]> build_request(const char* mountpoint,
                std::string name, std::string password);
        
        ntrip_client(const ntrip_client&) = delete;               /**< No copy constructor */
        ntrip_client(ntrip_client&&) = delete;                    /**< No move costructor */
        ntrip_client& operator=(const ntrip_client&) = delete;    /**< No copy operator */
        ntrip_client& operator=(ntrip_client&&) = delete;         /**< No move operator */

    public:
        ntrip_client() = default;
        ~ntrip_client() = default;

        /**
         * Download mount point table
         */
        std::variant<std::vector<mount_point>, io_status>
        getMountPoints(std::string address, int tcpPort, 
            std::string name = std::string(), std::string password = std::string());

        /**
         * Helper method to check if download is complete
         */
        bool hasTableEnding(std::string_view data);

        /**
         * Create connection with NTRIP Caster
         * @param nlogin login information
         * @return result of the connection
         */
        [[nodiscard]] status connect(ntrip_login& nlogin);

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
        [[nodiscard]] io_status send_gga_begin(location location, std::chrono::system_clock::time_point time);

        /**
         * @return status associated with AIOCBP.
         */
        [[nodiscard]] ssize_t send_end();

        /**
         * @return current status of the operation
         */
        [[nodiscard]] status get_status();
    };

}

#endif