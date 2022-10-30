#include <sys/types.h>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <sys/un.h>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>

#include "tcp_server.hpp"


namespace VrsTunnel::Ntrip
{

void tcp_server::stop() {
	stop_required = true;
	this->m_thread.join();
}

void tcp_server::task(uint16_t port, std::function<void(async_io)> client_connected, std::promise<bool>&& promise) {
	addrinfo hints{};
	::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;	/* TCP socket */
	hints.ai_flags = AI_PASSIVE;		/* For wildcard IP address */
	hints.ai_protocol = 0;			/* Any protocol */
	hints.ai_canonname = nullptr;
	hints.ai_addr = nullptr;
	hints.ai_next = nullptr;

	std::string port_str = std::to_string(port);
	addrinfo* result{nullptr};
	if (::getaddrinfo(nullptr, port_str.c_str(), &hints, &result) != 0) {
		promise.set_value(false);
		return;
	}

	/* getaddrinfo() returns a list of address structures.
	   Try each address until we successfully bind(2).
	   If socket(2) (or bind(2)) fails, we (close the socket
	   and) try the next address. */
	addrinfo* rp{nullptr};
	int srv_fd{-1};
	for (rp = result; rp != nullptr; rp = rp->ai_next) {
		srv_fd = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (srv_fd == -1) {
			continue;
		}

		int optval{1};
		int optname = SO_REUSEADDR | SO_REUSEPORT;
		if (::setsockopt(srv_fd, SOL_SOCKET, optname, &optval, sizeof(optval)) == -1) {
			continue;
		}

		if (::fcntl(srv_fd, F_SETFL, O_NONBLOCK) == -1) {
			continue;
		}

		if (::bind(srv_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
			break; /* Success */
		}

		::close(srv_fd);
	}

	::freeaddrinfo(result);

	if (rp == nullptr) { /* No address succeeded */
		promise.set_value(false);
		return;
	}

	constexpr int BACKLOG{25};
	if (::listen(srv_fd, BACKLOG) == -1) {
		promise.set_value(false);
		return;
	}

	promise.set_value(true);

	while (this->stop_required == false) {
		int cl_fd = ::accept(srv_fd, nullptr, nullptr);
		if (cl_fd > 0) {
			client_connected(async_io(cl_fd));
		} else {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
		}
	}
	::close(srv_fd);
}

[[nodiscard]] bool tcp_server::start(uint16_t port, std::function<void(async_io)> client_connected)
{
	std::promise<bool> barrier{};
	std::future<bool> future = barrier.get_future();
	m_thread = std::thread{&tcp_server::task, this, port, std::move(client_connected), std::move(barrier)};
	future.wait();
	return future.get();
}

}
