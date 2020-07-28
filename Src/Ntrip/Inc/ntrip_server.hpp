#ifndef VRSTUNNEL_NTRIP_NTRIP_SERVER_
#define VRSTUNNEL_NTRIP_NTRIP_SERVER_

#include "ntrip_login.hpp"
#include "ntrip_client.hpp"
#include <bits/c++config.h>

namespace VrsTunnel::Ntrip
{
    /**
     * NTRIP Server supplies GNSS correction 
     * from RTK base station to NTRIP Caster.
     */
    class ntrip_server
    {
    private:
        std::unique_ptr<async_io> m_aio {nullptr};      /**< Asyncronous operations */
        std::unique_ptr<tcp_client> m_tcp {nullptr};    /**< TCP connection */
        status m_status {status::uninitialized};        /**< Current status of the client */

        /**
         * Build HTTP POST request
         * @param nlogin NTRIP login data
         * @return HTTP POST null terminated request buffer
         */
        std::unique_ptr<char[]> build_request(const ntrip_login& nlogin);

    public:
        ntrip_server() = default;
        ~ntrip_server() = default;
        ntrip_server(const ntrip_server&) = delete;             /**< No copy constructor */
        ntrip_server(ntrip_server&&) = delete;                  /**< No move costructor */
        ntrip_server& operator=(const ntrip_server&) = delete;  /**< No copy operator */
        ntrip_server& operator=(ntrip_server&&) = delete;       /**< No move operator */

        /**
         * Create connection with NTRIP Caster
         * @param nlogin login information
         * @return result of the connection
         */
        [[nodiscard]] status connect(const ntrip_login& nlogin);

        /**
         * Disconnect from NTRIP Caster
         */
        void disconnect();

        /**
         * Send the correction to NTRIP Caster (asyncronously)
         * @param data correction buffer
         * @param size the buffer size
         * @return status of the operation
         */
        [[nodiscard]] status send_begin(const char* data, std::size_t size);

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