#ifndef VRS_TUNNEL_TCP_SERVER_HPP_
#define VRS_TUNNEL_TCP_SERVER_HPP_

#include <memory>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <functional>
#include <future>

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

	[[nodiscard]] bool start(uint16_t port, std::function<void(async_io)> client_connected);
	
 private:
	std::thread m_thread{};
	void task(uint16_t port, std::function<void(async_io)> client_connected, std::promise<bool>&& promise);
};

}

#endif /* VRS_TUNNEL_TCP_SERVER_HPP_ */
