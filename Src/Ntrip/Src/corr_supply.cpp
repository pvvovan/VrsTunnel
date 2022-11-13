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
			auto data = m_aio.read(static_cast<size_t>(avail));
			std::string str{data.get()};
			std::cout << "data: " << str << std::endl;
		} else {
			std::cout << "No data from server" << std::endl;
		}
		return true;
	}
	return false;
}

void corr_supply::close() {
	m_aio.close();
}

}
