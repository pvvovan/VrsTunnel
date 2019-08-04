#ifndef VRS_TUNNEL_CMD_LINE_
#define VRS_TUNNEL_CMD_LINE_

#include <variant>
#include <map>
#include <charconv>
#include <string>


namespace VrsTunnel
{

class cli
{
    public:
    using arg = std::variant<int, double, std::string>;

    private:
    std::map<std::string, arg> mParsedArgs;
    void parse_args(int argc, const char** argv);
    arg try_parse(std::string_view sv);

    public:
    explicit cli(int argc, const char** argv) 
    { 
        parse_args(argc, argv); 
    }

    std::optional<arg> find(const std::string& name) const;

};

cli::arg cli::try_parse(std::string_view sv)
{
    // try with int first
    int result = 0;
    const auto last = sv.data() + sv.size();
    const auto res = std::from_chars(sv.data(), last, result);
    if (res.ec != std::errc{} || res.ptr != last)
    {
        // if not possible, then just assume it's a double
        std::string sparam{sv};
        try {
            double value = std::stod(sparam);
            return value;
        }
        catch (...) { }
        // if still not possible, then just assume it's a string
        return sparam;
    }
    return result;
}

void cli::parse_args(int argc, const char** argv)
{
    // the form: -argName value -argName value
    for (int i = 1; i < argc; i+=2)
    {
        if (argv[i][0] != '-') { // super advanced pattern matching! :)
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

}

#endif /* VRS_TUNNEL_CMD_LINE_ */