#ifndef VRSTUNNEL_NTRIP_CORRECTION_SUPPLY_HPP_
#define VRSTUNNEL_NTRIP_CORRECTION_SUPPLY_HPP_

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <chrono>

#include "async_io.hpp"


namespace VrsTunnel::Ntrip
{

class corr_supply {
 public:
	explicit corr_supply(async_io&& aio, std::set<std::string>& auths);
	bool process();
	void close();

	~corr_supply()			= default;
	corr_supply(corr_supply&&)	= default;

	corr_supply(const corr_supply&)			= delete;
	corr_supply& operator=(const corr_supply&)	= delete;
	corr_supply& operator=(corr_supply&&)		= delete;

 private:
	async_io m_aio;

	enum class conn_state { auth, run };
	conn_state m_state {conn_state::auth};
	std::vector<char> m_auth_raw{};

	bool process_auth(std::unique_ptr<char[]> chunk, size_t len);
	bool process_corr(std::unique_ptr<char[]> chunk, size_t len);
	bool parse_auth();
	std::chrono::time_point<std::chrono::steady_clock> m_lastepoch;

	const std::set<std::string>& m_auths;
};

}

#endif /* VRSTUNNEL_NTRIP_CORRECTION_SUPPLY_HPP_ */
