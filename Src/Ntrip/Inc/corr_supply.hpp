#ifndef VRSTUNNEL_NTRIP_CORRECTION_SUPPLY_HPP_
#define VRSTUNNEL_NTRIP_CORRECTION_SUPPLY_HPP_

#include "async_io.hpp"


namespace VrsTunnel::Ntrip
{

class corr_supply {
 public:
	explicit corr_supply(async_io&& aio);

	~corr_supply()			= default;
	corr_supply(corr_supply&&)	= default;

	corr_supply(const corr_supply&)			= delete;
	corr_supply& operator=(const corr_supply&)	= delete;
	corr_supply& operator=(corr_supply&&)		= delete;

 private:
	async_io m_aio;
};

}

#endif /* VRSTUNNEL_NTRIP_CORRECTION_SUPPLY_HPP_ */
