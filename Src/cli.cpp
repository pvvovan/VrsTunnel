
#include <charconv>

#include "cli.hpp"

namespace VrsTunnel
{

    cli::arg cli::try_parse(std::string_view sv) const
    {
        /* Try with int first. */
        int result = 0;
        const auto last = sv.data() + sv.size();
        const auto res = std::from_chars(sv.data(), last, result);
        if (res.ec != std::errc{} || res.ptr != last)
        {
            /* If not possible, then assume it's a double. */
            std::string sparam{sv};
            try {
                std::size_t next = 0;
                double value = std::stod(sparam, &next);
                if (next != sparam.length()) {  /* If not all text is a number, */
                    return sparam;              /* just assume it's a string. */
                }
                return value;
            }
            catch (...) { }
            /* If still not possible, then just assume it's a string. */
            return sparam;
        }
        return result;
    }

    void cli::parse_args(int argc, const char* argv[])
    {
        /* Argument form: -argName value -argName value. */
        for (int i = 1; i < argc; i+=2)
        {
            if (argv[i][0] != '-') { /* dash based pattern matching */
                throw std::runtime_error("wrong command name");
            }
            mParsedArgs[argv[i]+1] = try_parse(argv[i+1]);
        }
    }

    std::optional<cli::arg> cli::find(const std::string& name) const
    {
        if (const auto it = mParsedArgs.find(name); it != mParsedArgs.end()) {
            return it->second;
        }
        return { };
    }

    double cli::to_double(const std::optional<VrsTunnel::cli::arg>& arg) const
    {
        if (std::holds_alternative<int>(*arg)) {
            return std::get<int>(*arg);
        }
        else {
            return std::get<double>(*arg);
        }
    }

}
