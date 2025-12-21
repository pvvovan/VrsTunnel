#ifndef VRSTUNNEL_NTRIP_BASE64_ENCODER_HPP_
#define VRSTUNNEL_NTRIP_BASE64_ENCODER_HPP_

#include <memory>

#include "login_encode.hpp"


namespace VrsTunnel::Ntrip
{
	using namespace std;

	/**
	* Concrete NTRIP encoder class
	*/
	class base64_encoder : public login_encode
	{
	private:
		constexpr base64_encoder() noexcept { };

	public:
		/**
		* Encode login data
		* @param name NTRIP login user name
		* @param password NTRIP login user password
		* @return encoded string
		*/
		string get(string name, string password) override;

		/**
		* @return instance of the encoder
		*/
		static unique_ptr<login_encode> make_instance();
		
	};
	
}

#endif /* VRSTUNNEL_NTRIP_BASE64_ENCODER_HPP_ */
