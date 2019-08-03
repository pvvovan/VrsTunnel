#include <iostream>
#include <cstdlib>

#include "CmdLine.hpp"
#include "NtripClient.hpp"

int print_usage() 
{
    std::cerr << "Usage: ntclient PARAMETERS..." << std::endl;
    std::cerr << "'ntclient' writes RTK correction to standard output." << std::endl << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "    ntclient -a caster.com -p 2101 -m CMR -u myname -pw myword -la 30.0 -lo -50.0" << std::endl;
    std::cerr << "    ntclient --address rtk.com --port 2101 --mount CMR --user myname --password myword --latitude 30.32 --longitude -52.65" << std::endl;
    std::cerr << "    ntclient -a caster.com -p 2101 -g y" << std::endl;
    std::cerr << "    ntclient --address rtk.com --port 2101 --user myname --password myword --get yes" << std::endl << std::endl;
    std::cerr << "Parameters:" << std::endl;
    std::cerr << "    -a,  --address SERVER         NTRIP Caster address" << std::endl;
    std::cerr << "    -p,  --port PORT              NTRIP Caster port" << std::endl;
    std::cerr << "    -m,  --mount MOUNTPOINT       NTRIP mount point" << std::endl;
    std::cerr << "    -u,  --user USERNAME          NTRIP user name" << std::endl;
    std::cerr << "    -pw, --password PASSWORD      NTRIP password" << std::endl;
    std::cerr << "    -la, --latitude LATITUDE      user location latitude" << std::endl;
    std::cerr << "    -lo, --longitude LONGITUDE    user location longitude" << std::endl;
    std::cerr << "    -g,  --get (y/n, yes/no)      retrieve mount points" << std::endl;
    return 1;
}

int showMountPoints(std::string address, int port, std::string username, std::string password)
{
    if (address.size() == 0 || port == 0) {
        return print_usage();
    }
    VrsTunnel::Ntrip::NtripClient nc{};
    auto res = nc.getMountPoints(address, port, username, password);
    if (std::holds_alternative<VrsTunnel::Ntrip::io_status>(res)) {
        std::cerr << "Error retreiving mount points" << std::endl;
    }
    else {
        auto mounts = std::get<std::vector<VrsTunnel::Ntrip::MountPoint>>(res);
        for(const auto& m : mounts) {
            std::cerr << m.Name << std::endl;
        }
    }
    
    return 0;
}

int main(int argc, const char* argv[])
{
    if (argc == 1) {
        return print_usage();
    }

    constexpr double noGeo = 1234536;
    double latitude{noGeo}, longitude{noGeo};
    std::string name{}, password{}, mount{}, address{}, yesno{};
    int port{0};

    try
    {
        VrsTunnel::CmdLine cmdLine(argc, argv);
        if (auto arg = cmdLine.Find("-address"); arg) {
            address = std::get<std::string>(*arg);
            // std::cout << "address is " << address << '\n';
        }
        if (auto arg = cmdLine.Find("a"); arg) {
            address = std::get<std::string>(*arg);
            // std::cout << "address is " << address << '\n';
        }
        if (auto arg = cmdLine.Find("-port"); arg) {
            port = std::get<int>(*arg);
            // std::cout << "port is " << port << '\n';
        }
        if (auto arg = cmdLine.Find("p"); arg) {
            port = std::get<int>(*arg);
            // std::cout << "port is " << port << '\n';
        }
        if (auto arg = cmdLine.Find("-mount"); arg) {
            mount = std::get<std::string>(*arg);
            // std::cout << "mount point is " << mount << '\n';
        }
        if (auto arg = cmdLine.Find("m"); arg) {
            mount = std::get<std::string>(*arg);
            // std::cout << "mount point is " << mount << '\n';
        }
        if (auto arg = cmdLine.Find("-user"); arg) {
            name = std::get<std::string>(*arg);
            // std::cout << "user name is " << name << '\n';
        }
        if (auto arg = cmdLine.Find("u"); arg) {
            name = std::get<std::string>(*arg);
            // std::cout << "user name is " << name << '\n';
        }
        if (auto arg = cmdLine.Find("-password"); arg) {
            password = std::get<std::string>(*arg);
            // std::cout << "password is " << password << '\n';
        }
        if (auto arg = cmdLine.Find("pw"); arg) {
            password = std::get<std::string>(*arg);
            // std::cout << "password is " << password << '\n';
        }
        if (auto arg = cmdLine.Find("-latitude"); arg) {
            latitude = atof((std::get<std::string>(*arg)).data());
            // std::cout << "latitude is " << latitude << '\n';
        }
        if (auto arg = cmdLine.Find("la"); arg) {
            latitude = atof((std::get<std::string>(*arg)).data());
            // std::cout << "latitude is " << latitude << '\n';
        }
        if (auto arg = cmdLine.Find("-longitude"); arg) {
            longitude = atof((std::get<std::string>(*arg)).c_str());
            // std::cout << "longitude is " << longitude << '\n';
        }
        if (auto arg = cmdLine.Find("lo"); arg) {
            longitude = atof((std::get<std::string>(*arg)).c_str());
            // std::cout << "longitude is " << longitude << '\n';
        }
        if (auto arg = cmdLine.Find("g"); arg) {
            yesno = std::get<std::string>(*arg);
            // std::cerr << "get = " << yesno << std::endl;
        }
        if (auto arg = cmdLine.Find("-get"); arg) {
            yesno = std::get<std::string>(*arg);
            // std::cerr << "get = " << yesno << std::endl;
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

    if (yesno.compare("yes") == 0 || yesno.compare("y") == 0) {
        return showMountPoints(address, port, name, password);
    }

    if (latitude == noGeo || longitude == noGeo || port == 0
            || address.size() == 0 || mount.size() == 0
            || name.size() == 0 || password.size() == 0) {
        return print_usage();
    }

    return 0;
}