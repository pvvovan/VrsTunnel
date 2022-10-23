#include <stdexcept>

#include "tcp_server.hpp"


namespace VrsTunnel::Ntrip
{
	template<typename connect_listen>
	[[nodiscard]] bool tcp_server::start(uint16_t port, connect_listen& listener)
	{
		m_port = port;
		auto start_thread = [instance = this](std::thread& the_thread, int& servfd, struct sockaddr_in& addr,
					sa_family_t family, connect_listen& listener, uint16_t port, std::atomic<bool>& stop_required) {
			servfd = ::socket(family, SOCK_STREAM, 0);
			if (servfd == 0) {
				servfd = -1;
				return false;
			}
			::memset(&addr, 0, sizeof(addr));
			addr.sin_family      = family;
			addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
			addr.sin_port        = ::htons(port);
			if ((::bind(servfd, (const struct sockaddr *)&addr, sizeof(addr))) < 0) {
				return false;
			}
			if (::listen(servfd, 64) < 0) {
				return false;
			}
			stop_required.store(false);
			the_thread = std::thread{&tcp_server::run_accepting<connect_listen>,
					instance, (struct sockaddr*)&addr, servfd, std::ref(listener)};
			return true;
		};

		return start_thread(m_thread_v4, m_servfd4, m_addr4, AF_INET, listener, m_port, stop_required);
	}

	template<typename connect_listen>
	void tcp_server::run_accepting(struct sockaddr* addr, int sockfd, connect_listen& listener)
	{
		socklen_t sock_len = sizeof(*addr);
		for(;;) {
			if (stop_required.load()) {
				return;
			}
			int client_socket = ::accept(sockfd, addr, &sock_len);
			if (client_socket > 0) {
				listener.OnClientConnected(std::make_unique<tcp_client>(client_socket));
			}
		}
	}
		
	void tcp_server::stop()
	{
		stop_required.store(true);
		if (m_port > 0) {
			tcp_client tc{-1};
			auto res = tc.connect("localhost", m_port);
			if (res == io_status::Success) {
				tc.close();
			}
		}
		this->close(m_servfd4);
		if (m_thread_v4.joinable()) {
			m_thread_v4.join();
		}
	}
	
	tcp_server::~tcp_server()
	{
		this->close(m_servfd4);
	}

	void tcp_server::close(int& servfd) {
		if (servfd != -1) {
			int res = ::close(servfd);
			if (res != 0) {
				throw std::runtime_error("possible memory leak");
			}
			servfd = -1;
		}
	}
}
