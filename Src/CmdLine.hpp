#ifndef VRS_TUNNEL_CMD_LINE_
#define VRS_TUNNEL_CMD_LINE_

#include <variant>
#include <map>
#include <charconv>
#include <string>


namespace VrsTunnel
{

class CmdLine
{
    public:
    using Arg = std::variant<int, float, std::string>;

    private:
    std::map<std::string, Arg> mParsedArgs;
    void ParseArgs(int argc, const char** argv);
    CmdLine::Arg TryParseString(std::string_view sv);

    public:
    explicit CmdLine(int argc, const char** argv) 
    { 
        ParseArgs(argc, argv); 
    }

    std::optional<Arg> Find(const std::string& name) const;

};

CmdLine::Arg CmdLine::TryParseString(std::string_view sv)
{
    // try with float first
    // float fResult = 0.0f;
    int fResult = 0;
    const auto last = sv.data() + sv.size();
    const auto res = std::from_chars(sv.data(), last, fResult);
    if (res.ec != std::errc{} || res.ptr != last)
    {
        // if not possible, then just assume it's a string
        return std::string{sv};
    }
    // no fraction part? then just cast to integer
    if (static_cast<int>(fResult) == fResult) {
        return static_cast<int>(fResult);
    }
    return fResult;
}

void CmdLine::ParseArgs(int argc, const char** argv)
{
    // the form: -argName value -argName value
    for (int i = 1; i < argc; i+=2)
    {
        if (argv[i][0] != '-') { // super advanced pattern matching! :)
            throw std::runtime_error("wrong command name");
        }
        mParsedArgs[argv[i]+1] = TryParseString(argv[i+1]);
    }
}

std::optional<CmdLine::Arg> CmdLine::Find(const std::string& name) const
{
    if (const auto it = mParsedArgs.find(name); it != mParsedArgs.end()) {
        return it->second;
    }
    return { };
}

}

#endif /* VRS_TUNNEL_CMD_LINE_ */