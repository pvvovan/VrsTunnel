#include <iostream>

#include "cli.hpp"
#include "NtripClient.hpp"

int print_usage() 
{
    std::cerr << "Usage: ntclient PARAMETERS..." << std::endl;
    std::cerr << "'ntclient' writes RTK correction to standard output." << std::endl << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "    ntclient -a rtk.ua -p 2101 -m mymount -u myname -pw myword -la 30 -lo -50" << std::endl;
    std::cerr << "    ntclient --address rtk.ua --port 2101 --mount CMR --user myname --password myword --latitude 30.32 --longitude -52.65" << std::endl;
    std::cerr << "    ntclient -a rtk.ua -p 2101 -g y" << std::endl;
    std::cerr << "    ntclient --address rtk.ua --port 2101 --user myname --password myword --get yes" << std::endl << std::endl;
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
        std::cout << "Error retreiving mount points" << std::endl;
    }
    else {
        auto mounts = std::get<std::vector<VrsTunnel::Ntrip::MountPoint>>(res);
        for(const auto& m : mounts) {
            std::cout << m.Name << "\t<" << m.Type << "\t(" << m.Reference.Latitude << ";" 
                    << m.Reference.Longitude << ")>" << std::endl;
        }
    }
    
    return 0;
}

void output_correction(VrsTunnel::Ntrip::ntrip_login login)
{
    VrsTunnel::Ntrip::NtripClient nc{};
    auto res = nc.connect(login);
    if (res == VrsTunnel::Ntrip::status::authfailure) {
        std::cerr << "ntclient: authentication failure" << std::endl;
        return;
    }
    else if (res == VrsTunnel::Ntrip::status::error) {
        std::cerr << "ntclient: connection error" << std::endl;
        return;
    }
    else if (res == VrsTunnel::Ntrip::status::nomount) {
        std::cerr << "ntclient: mount point not found" << std::endl;
        return;
    }

    constexpr int timeout_gga = 100; // 10 seconds (100 times 100ms)
    constexpr int timeout_status = 300; // 30 seconds (300 times 100ms)
    int time_gga = 0;
    int time_status = 0;
    auto sendgga = [&time_gga, &nc, &login]() {
        time_gga = 0;
        auto time = std::chrono::system_clock::now();
        auto send_res = nc.send_gga(login.position, time);
        if (send_res != VrsTunnel::Ntrip::io_status::Success) {
            std::cerr << "ntclient: gga sending error" << std::endl;
        }
    };
    sendgga();
    for (;;) {
        ++time_gga;
        if (time_gga == timeout_gga && nc.get_status() == VrsTunnel::Ntrip::status::ready) {
            nc.send_end();
            sendgga();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int avail = nc.available();
        if (avail > 0) {
            auto corr = nc.receive(avail);
            fwrite(corr.get(), avail, 1, stdout);
            fflush(stdout);
        }

        ++time_status;
        if (time_status == timeout_status) {
            auto stat = nc.get_status();
            switch (stat)
            {
            case VrsTunnel::Ntrip::status::ready:
            case VrsTunnel::Ntrip::status::sending:
                time_status = 0;
                break;
            
            default:
                std::cerr << "ntclient: error" << std::endl;
                nc.disconnect();
                return;
                break;
            }
        }
    }
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
        VrsTunnel::cli cli(argc, argv);
        auto to_double = [](std::optional<VrsTunnel::cli::arg>& arg) -> double {
            if (std::holds_alternative<int>(*arg)) {
                return std::get<int>(*arg);
            }
            else {
                return std::get<double>(*arg);
            }
        };

        if (auto arg = cli.find("-address"); arg) {
            address = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("a"); arg) {
            address = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("-port"); arg) {
            port = std::get<int>(*arg);
        }
        if (auto arg = cli.find("p"); arg) {
            port = std::get<int>(*arg);
        }
        if (auto arg = cli.find("-mount"); arg) {
            mount = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("m"); arg) {
            mount = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("-user"); arg) {
            name = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("u"); arg) {
            name = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("-password"); arg) {
            password = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("pw"); arg) {
            password = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("-latitude"); arg) {
            latitude = to_double(arg);
        }
        if (auto arg = cli.find("la"); arg) {
            latitude = to_double(arg);
        }
        if (auto arg = cli.find("-longitude"); arg) {
            longitude = to_double(arg);
        }
        if (auto arg = cli.find("lo"); arg) {
            longitude = to_double(arg);
        }
        if (auto arg = cli.find("g"); arg) {
            yesno = std::get<std::string>(*arg);
        }
        if (auto arg = cli.find("-get"); arg) {
            yesno = std::get<std::string>(*arg);
        }
    }
    catch (std::bad_variant_access& err)
    {
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

    VrsTunnel::Ntrip::ntrip_login login{};
    login.address = address;
    login.port = port;
    login.mountpoint = mount;
    login.username = name;
    login.password = password;
    login.position.Latitude = latitude;
    login.position.Longitude = longitude;
    for (;;) {
        output_correction(login);
        constexpr int retry_period = 30;
        std::cerr << "ntclient: retrying in " << retry_period << " seconds" << std::endl;
        sleep(retry_period);
    }
    return 0;
}