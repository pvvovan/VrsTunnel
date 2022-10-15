#ifndef VRSTUNNEL_NTRIP_LOCATION_HPP_
#define VRSTUNNEL_NTRIP_LOCATION_HPP_

namespace VrsTunnel::Ntrip
{
	/**
	* Struct for geodetic coordinates
	*/
	struct location
	{
		double Latitude;
		double Longitude;
		double Elevation;

		constexpr location() noexcept :
			Latitude { 0 },
			Longitude { 0 },
			Elevation { 0 }
			{ }

		constexpr location(double latitude, double longitude, double elevation) noexcept :
			Latitude { latitude },
			Longitude { longitude },
			Elevation { elevation }
			{ }
	};
	
}

#endif /* _VRSTUNNEL_NTRIP_LOCATION_HPP_ */
