#include <memory>
#include <string_view>
#include <sstream>

#include "corr_supply.hpp"


#include <iostream>
#include <string>
namespace VrsTunnel::Ntrip
{

corr_supply::corr_supply(async_io&& aio) : m_aio{std::move(aio)} { }

bool corr_supply::process() {
	if (m_aio.check() == io_status::Success) {
		int avail = m_aio.available();
		if (avail > 0) {
			size_t len = static_cast<size_t>(avail);
			std::unique_ptr<char[]> chunk = m_aio.read(len);
			if (m_state == conn_state::auth) {
				return process_auth(std::move(chunk), len);
			} else {
				return process_corr(std::move(chunk), len);
			}
		} else {
			std::cout << "No data from server" << std::endl;
		}
		return true;
	}
	std::cout << "supply error" << std::endl;
	return false;
}

void corr_supply::close() {
	m_aio.close();
}

bool corr_supply::parse_auth()
{
	std::string str(&m_auth_raw[0], m_auth_raw.size());
	std::stringstream ss{str};
	std::string line{};
	int c{0};
	while (ss >> line) {
		c++;
		if (c == 13) {
			std::string auth = line;
			std::cout << auth << std::endl;
		}
	}
	return true;
}

bool corr_supply::process_auth(std::unique_ptr<char[]> chunk, size_t len) {
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

bool corr_supply::process_corr(std::unique_ptr<char[]> chunk, size_t len) {
	return true;
}

}
