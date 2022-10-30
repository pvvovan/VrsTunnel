#ifndef VRSTUNNEL_NTRIP_DISPATCHER_HPP_
#define VRSTUNNEL_NTRIP_DISPATCHER_HPP_

#include <cstdint>
#include <set>
#include <string>
#include <map>

#include "tcp_server.hpp"
#include "async_io.hpp"


namespace VrsTunnel::Ntrip
{

class dispatcher {
 public:
	dispatcher()		= default;
	~dispatcher()		= default;
	dispatcher(const dispatcher&)			= delete;
	dispatcher& operator=(const dispatcher&)	= delete;
	dispatcher(dispatcher&&)			= delete;
	dispatcher& operator=(dispatcher&&)		= delete;

	[[nodiscard]] bool start(uint16_t cli_port,
				std::map<std::string, std::set<std::string>> cli_auth,
				uint16_t srv_port,
				std::set<std::string> srv_auth);

	void stop();

 private:
	tcp_server m_cli_tcp{};
	tcp_server m_srv_tcp{};
	void client_connected(async_io client);
	void server_connected(async_io server);
	std::map<std::string, std::set<std::string>> m_cli_auth;
	std::set<std::string> m_srv_auth;
};

}

#endif /* VRSTUNNEL_NTRIP_DISPATCHER_HPP_ */
