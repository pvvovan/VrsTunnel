#ifndef VRSTUNNEL_NTRIP_CORRECTION_CONSUME_HPP_
#define VRSTUNNEL_NTRIP_CORRECTION_CONSUME_HPP_

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <chrono>
#include <map>
#include <string>

#include "async_io.hpp"


namespace VrsTunnel::Ntrip
{

class corr_consume {
 public:
	explicit corr_consume(async_io&& aio, std::map<std::string, std::set<std::string>>& auths);
	bool process();
	void close();

	~corr_consume()			= default;
	corr_consume(corr_consume&&)	= default;

	corr_consume(const corr_consume&)		= delete;
	corr_consume& operator=(const corr_consume&)	= delete;
	corr_consume& operator=(corr_consume&&)		= delete;

 private:
	async_io m_aio;

	enum class conn_state { auth, run, send_mounts, wait_mounts };
	conn_state m_state {conn_state::auth};
	std::vector<char> m_auth_raw{};

	bool process_auth(std::unique_ptr<char[]> chunk, size_t len);
	bool parse_auth();
	bool send_mount_points();
	std::chrono::time_point<std::chrono::steady_clock> m_lastepoch;

	const std::map<std::string, std::set<std::string>>& m_auths;
};

}

#endif /* VRSTUNNEL_NTRIP_CORRECTION_CONSUME_HPP_ */
