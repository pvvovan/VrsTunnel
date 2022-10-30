#include "corr_supply.hpp"


namespace VrsTunnel::Ntrip
{

corr_supply::corr_supply(async_io&& aio) : m_aio{std::move(aio)} { }

}
