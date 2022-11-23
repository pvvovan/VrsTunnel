#include <sys/epoll.h>
#include <memory>

#include "dispatcher.hpp"


#include <iostream>
#include <chrono>
namespace VrsTunnel::Ntrip
{

[[nodiscard]] bool dispatcher::start(uint16_t cli_port,
					std::map<std::string, std::set<std::string>> cli_auth,
					uint16_t srv_port,
					std::set<std::string> srv_auth)
{
	std::function<void(async_io)> cl_con = [this] (async_io client) {
		this->client_connected(std::move(client));
	};
	if (m_cli_tcp.start(cli_port, std::move(cl_con)) == false) {
		return false;
	}

	std::function<void(async_io)> sv_con = [this] (async_io server) {
		this->server_connected(std::move(server));
	};
	if (m_srv_tcp.start(srv_port, std::move(sv_con)) == false) {
		return false;
	}

	m_cli_auth = cli_auth;
	m_srv_auth = srv_auth;

	constexpr int EPOLL_SRV_SIZE {100};
	m_epoll_srvfd = ::epoll_create(EPOLL_SRV_SIZE);
	if (m_epoll_srvfd < 0) {
		return false;
	}
	m_srv_thread = std::thread{&dispatcher::server_processing, this};

	constexpr int EPOLL_CLI_SIZE {1000};
	m_epoll_clifd = ::epoll_create(EPOLL_CLI_SIZE);
	if (m_epoll_clifd < 0) {
		return false;
	}
	m_cli_thread = std::thread{&dispatcher::client_processing, this};

	return true;
}

void dispatcher::client_processing() {
	for ( ; ; ) {
		constexpr size_t MAX_CEVS {1000};
		epoll_event evlist[MAX_CEVS];
		int res = epoll_wait(m_epoll_clifd, &evlist[0], MAX_CEVS, 5000);
		if (res > 0) {
			for (int i = 0; i < res; i++) {
				if (evlist[i].events & EPOLLIN) {
					corr_consume* consume = static_cast<corr_consume*>(evlist[i].data.ptr);
					if (consume->process() == false) {
						consume->close();
						std::scoped_lock<std::mutex> sl {m_consumers_lock};
						m_consumers.remove_if([&consume] (auto& elem) {
							return elem.get() == consume;
						});
					}
				} else {
					std::cout << "cli epoll event error" << std::endl;
				}
			}
		} else if (res == 0) {
			std::cout << "cli epoll_wait timeout" << std::endl;
		} else {
			std::cout << "cli epoll_wait error" << std::endl;
		}
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1000ms);
	}
}

void dispatcher::client_connected(async_io client) {
	std::cout << "Client connected\n";
	const int client_fd = client.get_fd();
	auto consumer = std::make_unique<corr_consume>(std::move(client), m_cli_auth);
	if (consumer->process()) {
		epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.ptr = consumer.get();
		if (::epoll_ctl(m_epoll_clifd, EPOLL_CTL_ADD, client_fd, &ev) == 0) {
			std::scoped_lock<std::mutex> sl {m_consumers_lock};
			m_consumers.emplace_back(std::move(consumer));
		}
	}
}

void dispatcher::server_processing()
{
	for ( ; ; ) {
		constexpr size_t MAX_SEVS {100};
		epoll_event evlist[MAX_SEVS];
		int res = epoll_wait(m_epoll_srvfd, &evlist[0], MAX_SEVS, 5000);
		if (res > 0) {
			for (int i = 0; i < res; i++) {
				if (evlist[i].events & EPOLLIN) {
					corr_supply* supply = static_cast<corr_supply*>(evlist[i].data.ptr);
					if (supply->process() == false) {
						supply->close();
						std::scoped_lock<std::mutex> sl {m_suppliers_lock};
						m_suppliers.remove_if([&supply] (auto& elem) {
							return elem.get() == supply;
						});
					}
				} else {
					std::cout << "srv epoll event error" << std::endl;
				}
			}
		} else if (res == 0) {
			std::cout << "srv epoll_wait timeout" << std::endl;
		} else {
			std::cout << "srv epoll_wait error" << std::endl;
		}
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(100ms);
	}
}

void dispatcher::server_connected(async_io server) {
	const int server_fd = server.get_fd();
	auto supply = std::make_unique<corr_supply>(std::move(server), m_srv_auth);
	if (supply->process()) {
		epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.ptr = supply.get();
		if (::epoll_ctl(m_epoll_srvfd, EPOLL_CTL_ADD, server_fd, &ev) == 0) {
			std::scoped_lock<std::mutex> sl {m_suppliers_lock};
			m_suppliers.emplace_back(std::move(supply));
		}
	}
}

}
