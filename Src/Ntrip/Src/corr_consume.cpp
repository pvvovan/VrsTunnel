#include <memory>
#include <string_view>
#include <sstream>

#include "corr_consume.hpp"


#include <iostream>
#include <string>
namespace VrsTunnel::Ntrip
{

corr_consume::corr_consume(async_io&& aio, std::map<std::string, std::set<std::string>>& auths) :
				m_aio {std::move(aio)},
				m_lastepoch {std::chrono::steady_clock::now()},
				m_auths {auths}
				{ }

bool corr_consume::process() {
	if (m_aio.check() == io_status::Success) {
		int avail = m_aio.available();
		if (avail > 0) {
			m_lastepoch = std::chrono::steady_clock::now();
			size_t len = static_cast<size_t>(avail);
			std::unique_ptr<char[]> chunk = m_aio.read(len);
			if (m_state == conn_state::auth) {
				return process_auth(std::move(chunk), len);
			} else {
				// return process_corr(std::move(chunk), len);
			}
		} else {
			using namespace std::chrono_literals;
			if ((std::chrono::steady_clock::now() - m_lastepoch) > 20s) {
				std::cout << "No data from ntrip client" << std::endl;
				return false;
			}
		}
		return true;
	}
	std::cout << "coor_consume error" << std::endl;
	return false;
}

void corr_consume::close() {
	m_aio.close();
}

bool corr_consume::parse_auth()
{
	std::string str(&m_auth_raw[0], m_auth_raw.size());
	std::stringstream ss{str};
	std::string line{};
	int c{0};
	while (ss >> line) {
		c++;
		if (c == 13) {
			std::string auth = line;
			if (auto search = m_auths.find(auth); search != m_auths.end()) {
				std::cout << "Client Found " << auth << '\n';
				constexpr std::string_view resp {"HTTP/1.1 200 OK\r\n\r\n"};
				if (m_aio.write(resp.data(), resp.size()) == io_status::Success) {
					m_state = conn_state::run;
					return true;
				}
			} else {
				std::cout << "Client Not found\n";
				constexpr std::string_view resp {"HTTP/1.1 401 Unauthorized\r\n\r\n"};
				if (m_aio.write(resp.data(), resp.size()) == io_status::Success) {
					return true; // wait for timeout?
				}
			}
		}
	}
	return false;
}

bool corr_consume::process_auth(std::unique_ptr<char[]> chunk, size_t len) {
	m_auth_raw.resize(m_auth_raw.size() + len);
	for (size_t i = 0; i < len; i++) {
		m_auth_raw.emplace_back(chunk[i]);
	}
	if (m_auth_raw.size() > 100000) {
		return false;
	}
	constexpr std::string_view auth_ending {"Transfer-Encoding: chunked"};
	constexpr size_t endling_len = auth_ending.size();
	if (m_auth_raw.size() > auth_ending.size()) {
		const std::string_view ending(&m_auth_raw.back() - endling_len + 1, endling_len);
		if (ending.compare(auth_ending) == 0) {
			std::cout << "RTK base connected" << std::endl;
			return parse_auth();
		} else {
			std::cout << "ending: " << ending << std::endl;
			return false;
		}
	}
	return true;
}

// bool corr_consume::process_corr(std::unique_ptr<char[]> chunk, size_t len) {
// 	std::string str {chunk.get(), len};
// 	std::cout << str << std::flush;
// 	return true;
// }

}
