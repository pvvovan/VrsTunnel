#include <memory>
#include <string_view>
#include <sstream>

#include "corr_consume.hpp"


#include <iostream>
#include <string>
#include <chrono>
#include <thread>
namespace VrsTunnel::Ntrip
{

corr_consume::corr_consume(async_io&& aio, std::map<std::string, std::set<std::string>>& auths) :
				m_aio {std::move(aio)},
				m_lastepoch {std::chrono::steady_clock::now()},
				m_auths {auths}
				{ }

corr_consume::state_t corr_consume::process() {
	if (m_aio.check() == io_status::Success) {
		int avail = m_aio.available();
		if (avail > 0) {
			m_lastepoch = std::chrono::steady_clock::now();
			size_t len = static_cast<size_t>(avail);
			std::unique_ptr<char[]> chunk = m_aio.read(len);
			if (m_state == state_t::auth) {
				return process_auth(std::move(chunk), len);
			} else if (m_state == state_t::wait_mounts) {
				return send_mount_points();
			} else {
				// return process_corr(std::move(chunk), len);
			}
		} else {
			using namespace std::chrono_literals;
			if ((std::chrono::steady_clock::now() - m_lastepoch) > 20s) {
				std::cout << "No data from ntrip client" << std::endl;
				return state_t::disconnect;
			}
		}
		return m_state;
	}
	std::cout << "coor_consume error" << std::endl;
	return state_t::disconnect;
}

void corr_consume::close() {
	m_aio.close();
}

corr_consume::state_t corr_consume::parse_auth()
{
	std::string str(&m_auth_raw[0], m_auth_raw.size());
	std::stringstream ss{str};
	std::string line{};
	int c{0};
	while (ss >> line) {
		c++;
		if ((c == 2) && (line.compare("/") == 0)) {
			m_state = state_t::send_mounts;
			return send_mount_points();
		}
		if (c == 13) {
			std::string auth = line;
			if (auto search = m_auths.find(auth); search != m_auths.end()) {
				std::cout << "Client Found " << auth << '\n';
				constexpr std::string_view resp {"HTTP/1.1 200 OK\r\n\r\n"};
				if (m_aio.write(resp.data(), resp.size()) == io_status::Success) {
					m_state = state_t::run;
					return m_state;
				}
			} else {
				std::cout << "Client Not found\n";
				constexpr std::string_view resp {"HTTP/1.1 401 Unauthorized\r\n\r\n"};
				if (m_aio.write(resp.data(), resp.size()) == io_status::Success) {
					return state_t::run; // wait for timeout?
				}
			}
		}
	}
	return state_t::disconnect;
}

corr_consume::state_t corr_consume::process_auth(std::unique_ptr<char[]> chunk, size_t len) {
	m_auth_raw.resize(m_auth_raw.size() + len);
	for (size_t i = 0; i < len; i++) {
		m_auth_raw.emplace_back(chunk[i]);
	}
	if (m_auth_raw.size() > 100000) {
		return state_t::disconnect;
	}
	constexpr std::string_view auth_ending {"\r\n\r\n"};
	constexpr size_t endling_len = auth_ending.size();
	if (m_auth_raw.size() > auth_ending.size()) {
		const std::string_view ending(&m_auth_raw.back() - endling_len + 1, endling_len);
		if (ending.compare(auth_ending) == 0) {
			std::cout << "Ntrip client connected" << std::endl;
			return parse_auth();
		} else {
			std::cout << "Client ending: " << ending << std::endl;
			return state_t::disconnect;
		}
	}
	return m_state;
}

corr_consume::state_t corr_consume::send_mount_points() {
	if (m_state == state_t::send_mounts) {
		m_state = state_t::wait_mounts;
		constexpr std::string_view mount_resp {
			"HTTP/1.0 200 OK\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 97\r\n\r\n"
			"STR;tunnel;tunnel;RTCM 3;;2;GPS+GLO;;;55.55;33.33;1;1;VRS Tunnel;none;B;Y;9600;\r\n"
			"ENDSOURCETABLE\r\n"
		};
		if (m_aio.write(mount_resp.data(), mount_resp.size()) == io_status::Success) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);
			m_aio.close();
			std::cout << "get mounts success\n";
			return state_t::wait_mounts;
		} else {
			std::cout << "get mounts error\n";
		}
	} else {
		io_status status = m_aio.check();
		if (status == io_status::InProgress) {
			return m_state;
		} else if (status == io_status::Success) {
			static_cast<void>(m_aio.end());
			return state_t::disconnect;
		} else {
			return state_t::disconnect;
		}
	}
	return state_t::disconnect;
}

int corr_consume::get_fd() {
	return m_aio.get_fd();
}

}
