#ifndef VRS_TUNNEL_TCP_SERVER_HPP_
#define VRS_TUNNEL_TCP_SERVER_HPP_

#include <memory>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <functional>

#include "async_io.hpp"


namespace VrsTunnel::Ntrip
{
	class tcp_server
	{
	public:
		tcp_server() = default;
		tcp_server(const tcp_server&)			= delete;
		tcp_server(tcp_server&&)			= delete;
		tcp_server& operator=(const tcp_server&)	= delete;
		tcp_server& operator=(tcp_server&&)		= delete;

		[[nodiscard]] bool start(
			uint16_t port, const std::function<void(async_io)>& client_connected);

		void stop();
		
	private:
		// uint16_t m_port{};
		std::thread m_thread{};
		void task(uint16_t srv_fd, const std::function<void(async_io)>& client_connected);
		std::atomic<bool> stop_required{false};
		// int m_servfd4{-1};
		// struct sockaddr_in m_addr4{};
		// void close(int& servfd);

		// // template<typename connect_listen>
		// void run_accepting(struct sockaddr* addr, int sockfd, connect_listen& listener);
	};
}

#endif /* VRS_TUNNEL_TCP_SERVER_HPP_ */
