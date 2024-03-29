#ifndef TCP_ASYNCHRONOUS_CLIENT_HPP_
#define TCP_ASYNCHRONOUS_CLIENT_HPP_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "async_io.hpp"


namespace VrsTunnel::Ntrip
{
	/**
	 * TCP client class. Copy and move operations are disabled.
	 */
	class tcp_client
	{
	private:
		int m_sockfd{-1}; /**< socket file descriptor */

	public:
		/**
		 * constructor for existing socket file descriptor
		 */
		explicit tcp_client(int fd) noexcept;

		tcp_client(const tcp_client&)               = delete;
		tcp_client& operator=(const tcp_client&)    = delete;
		tcp_client(tcp_client&&)                    = delete;
		tcp_client& operator=(tcp_client&&)         = delete;

		/**
		 * desctructor closes TCP connection.
		 */
		~tcp_client();

		/**
		 * closes TCP connection if it is open.
		 */
		void close();

		/**
		 * connect method creates tcp connection.
		 * @param addr server address
		 * @param port TCP port of the server
		 * @return connection result
		 */
		[[nodiscard]] io_status connect(std::string addr, uint16_t port);

		/**
		 * @return file descriptor of TCP connection
		 */
		int get_sockfd() noexcept;
	};
}

#endif /* TCP_ASYNCHRONOUS_CLIENT_HPP_ */
