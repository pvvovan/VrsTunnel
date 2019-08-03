#include <iostream>

#include "CmdLine.hpp"

int print_usage() 
{
    std::cerr << "Usage: ntclient PARAMETERS..." << std::endl << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "    ntclient -a caster.com -p 2101 -m CMR -u testname -pw password -la 30.0 -lo -50.0" << std::endl;
    std::cerr << "    ntclient --address caster.com --port 2101 --mount CMR --user testname --password password --latitude 30.32 --longitude -52.365" << std::endl << std::endl;
    std::cerr << "Parameters:" << std::endl;
    std::cerr << "    -a,  --address SERVER         NTRIP Caster address" << std::endl;
    std::cerr << "    -p,  --port PORT              NTRIP Caster port" << std::endl;
    std::cerr << "    -m,  --mount MOUNTPOINT       NTRIP mount point" << std::endl;
    std::cerr << "    -u,  --user USERNAME          NTRIP user name" << std::endl;
    std::cerr << "    -pw, --password PASSWORD      NTRIP password" << std::endl;
    std::cerr << "    -la, --latitude LATITUDE      user location latitude" << std::endl;
    std::cerr << "    -lo, --longitude LONGITUDE    user location longitude" << std::endl;
    return 1;
}

int main(int argc, const char* argv[])
{
    if (argc == 1) {
        return print_usage();
    }
    try
    {
        VrsTunnel::CmdLine cmdLine(argc, argv);
        if (auto arg = cmdLine.Find("-address"); arg) {
            std::cout << "address is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("a"); arg) {
            std::cout << "address is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("-port"); arg) {
            std::cout << "port is " << std::get<int>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("p"); arg) {
            std::cout << "port is " << std::get<int>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("-mount"); arg) {
            std::cout << "mount point is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("m"); arg) {
            std::cout << "mount point is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("-user"); arg) {
            std::cout << "user name is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("u"); arg) {
            std::cout << "user name is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("-password"); arg) {
            std::cout << "password is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("pw"); arg) {
            std::cout << "password is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("-latitude"); arg) {
            std::cout << "latitude is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("la"); arg) {
            std::cout << "latitude is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("-longitude"); arg) {
            std::cout << "longitude is " << std::get<std::string>(*arg) << '\n';
        }
        if (auto arg = cmdLine.Find("lo"); arg) {
            std::cout << "longitude is " << std::get<std::string>(*arg) << '\n';
        }
        // if (auto arg = cmdLine.Find("paramInt"); arg) {
        //     std::cout << "paramInt is " << std::get<int>(*arg) << '\n';
        // }
        // if (auto arg = cmdLine.Find("paramFloat"); arg)
        // {
        //     if (const auto intPtr = std::get_if<int>(&*arg); intPtr) {
        //         std::cout << "1paramFloat is " << *intPtr << " (integer)\n";
        //     }
        //     else {
        //         std::cout << "2paramFloat is " << std::get<float>(*arg) << '\n';
        //     }
        // }
        // if (auto arg = cmdLine.Find("paramText"); arg) {
        //     std::cout << "paramText is " << std::get<std::string>(*arg) << '\n';
        // }
    }
    catch (std::bad_variant_access& err)
    {
        // std::cerr << " ...err: accessing a wrong variant type, "
        //         << err.what() << '\n';
        return print_usage();
    }
    catch (std::runtime_error &err)
    {
        std::cerr << " ...err: " << err.what() << std::endl;;
        return print_usage();
    }

    return 0;
}