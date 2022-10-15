#ifndef VRSTUNNEL_NTRIP_NMEA_HPP_
#define VRSTUNNEL_NTRIP_NMEA_HPP_

#include <string>
#include <charconv>
#include <chrono>
#include <variant>

#include "location.hpp"


namespace VrsTunnel::Ntrip
{
	/**
	* NMEA helper class with static methods
	*/
	class nmea
	{
	private:
		constexpr nmea() noexcept { };

	public:
		/**
		* Operation result
		*/
		enum class [[nodiscard]] ErrorCode
		{
			Undefined /**< currently the ony one */
		};

		/**
		* This method generates NMEA GGA sentence.
		* NMEA GGA message generation based on provided location and time,
		* speed is assumed to be 0, correction source is RTK, 12 satellies,
		* DOP 0.9, age of DGPS data record and reference station ID are 0,
		* both heights are equal to elevetion.
		* @param location geodetic coordinates
		* @param time epoch of the location
		* @return NMEA GGA sentence
		*/
		[[nodiscard]] static std::variant<std::string, ErrorCode>
		getGGA(location location, std::chrono::system_clock::time_point time);

		/**
		* Compute checksum of NMEA sentence
		* @param data byte stream
		* @return checksum of the data
		*/
		static uint8_t checksum(std::string_view data);
	};
}

#endif /* VRSTUNNEL_NTRIP_NMEA_HPP_ */
