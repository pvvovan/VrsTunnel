#ifndef VRS_TUNNEL_CLI_LINE_HPP_
#define VRS_TUNNEL_CLI_LINE_HPP_

#include <variant>
#include <optional>
#include <map>
#include <string>
#include <vector>
#include <sstream>


namespace VrsTunnel
{
	/**
	 * Helper class to parse command line arguments
	 */
	class cli final
	{
	public:
		using arg = std::variant<int, double, std::string>; /**< Generic argument type */

	private:
		std::map<std::string, arg> mParsedArgs; /**< All arguments are stored here */
		void parse_args(int argc, char const *const *const argv);
		arg try_parse(std::string_view sv) const;

		/**
		 * If double type parameter is passed as int, it will parsed as int.
		 * Therefore, both type are checked!
		 */
		double to_double(const std::optional<VrsTunnel::cli::arg>& arg) const;

		/**
		 * Find argument by its name
		 */
		std::optional<arg> find(const std::string& name) const;

		public:
		explicit cli(int argc, char const *const *const argv);
		~cli() = default;
		
		/**
		 * Retrieve parameter by it's name.
		 * List of names is supported: {"long_name, ln"}.
		 */
		template<typename T>
		bool retrieve(std::vector<std::string> names, T& value) const
		{
			for (const auto& n : names) {
				if constexpr (std::is_floating_point_v<T>) {
					if (const auto arg = this->find(n); arg) {
						value = to_double(arg);
						return true;
					}
				}
				else if constexpr (std::is_same_v<T, std::string>) {
					/**
					 * If the expected type is string but the value was parsed
					 * as int or double, it is necessary to convert it back.
					 */
					if (const auto arg = this->find(n); arg) {
						if (const auto intPtr = std::get_if<int>(&*arg); intPtr) {
							value = std::to_string(*intPtr);
						}
						else if (const auto doublePtr = std::get_if<double>(&*arg); doublePtr) {
							std::ostringstream os;
							os << *doublePtr;
							value = os.str();
						}
						else {
							value = std::move(std::get<T>(*arg));
						}
						return true;
					}
				}
				else {
					if (const auto arg = this->find(n); arg) {
						value = std::get<T>(*arg);
						return true;
					}
				}
			}
			return false;
		}
	};
}

#endif /* VRS_TUNNEL_CLI_LINE_HPP_ */
