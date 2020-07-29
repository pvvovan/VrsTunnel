#ifndef VRSTUNNEL_NTRIP_LOGIN_ENCODE_
#define VRSTUNNEL_NTRIP_LOGIN_ENCODE_

#include <string>
#include <memory>

namespace VrsTunnel::Ntrip
{
	using std::string;

	/**
	* Abstract class for NTRIP login encoding
	*/
	class login_encode
	{
	public:
		login_encode() = default;
		virtual ~login_encode() = default;
		virtual string get(string name, string password) = 0;

		/**
		* Factory method to obtain NTRIP login encoder
		*/
		static std::unique_ptr<login_encode> make_instance();
	};
}

#endif /* __VRSTUNNEL_NTRIP_LOGIN_ENCODE_ */
