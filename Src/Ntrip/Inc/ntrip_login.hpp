#ifndef VRSTUNNEL_NTRIP_NTRIP_LOGIN_HPP_
#define VRSTUNNEL_NTRIP_NTRIP_LOGIN_HPP_

#include <string>
#include <cstdint>

#include "location.hpp"


namespace VrsTunnel::Ntrip
{
	/**
	* Here we can have NTRIP login information
	*/
	struct ntrip_login
	{
		std::string address;    /**< NTRIP Caster address */
		uint16_t port;               /**< NTRIP Caster TCP port */
		std::string username;
		std::string password;
		std::string mountpoint;
		location position;      /**< Coordinates to be sent to NTRIP Caster */
	};
	
}

#endif /* VRSTUNNEL_NTRIP_NTRIP_LOGIN_HPP_ */
