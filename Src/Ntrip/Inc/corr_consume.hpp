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
	enum class state_t { auth, run, send_mounts, wait_mounts, disconnect };

	explicit corr_consume(async_io&& aio, std::map<std::string, std::set<std::string>>& auths);
	state_t process();
	void close();
	int get_fd();

	~corr_consume()			= default;
	corr_consume(corr_consume&&)	= default;

	corr_consume(const corr_consume&)		= delete;
	corr_consume& operator=(const corr_consume&)	= delete;
	corr_consume& operator=(corr_consume&&)		= delete;

 private:
	async_io m_aio;

	state_t m_state {state_t::auth};
	std::vector<char> m_auth_raw{};

	state_t process_auth(std::unique_ptr<char[]> chunk, size_t len);
	state_t parse_auth();
	state_t send_mount_points();
	std::chrono::time_point<std::chrono::steady_clock> m_lastepoch;

	const std::map<std::string, std::set<std::string>>& m_auths;
};

}

#endif /* VRSTUNNEL_NTRIP_CORRECTION_CONSUME_HPP_ */
