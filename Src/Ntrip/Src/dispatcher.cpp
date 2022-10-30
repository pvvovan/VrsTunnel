#include "dispatcher.hpp"


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
	return true;
}

void dispatcher::client_connected(async_io client) {

}

void dispatcher::server_connected(async_io server) {
	m_suppliers.emplace_back(corr_supply(std::move(server)));
}

}
