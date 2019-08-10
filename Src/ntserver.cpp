#include <iostream>

#include "cli.hpp"
#include "ntrip_server.hpp"

void send_correction(VrsTunnel::Ntrip::ntrip_login& login);

int print_usage() 
{
    std::cerr << "Usage: ntserver PARAMETERS..." << std::endl;
    std::cerr << "'ntserver' reads RTK correction from standard input." << std::endl << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "    ntserver -a rtk.ua -p 2101 -m mymount -u myname -pw myword -la 30 -lo -50" << std::endl;
    std::cerr << "    ntserver --address rtk.ua --port 2101 --mount CMR --user myname --password myword --latitude 30.32 --longitude -52.65" << std::endl;
    std::cerr << "Parameters:" << std::endl;
    std::cerr << "    -a,  --address SERVER         NTRIP Caster address" << std::endl;
    std::cerr << "    -p,  --port PORT              NTRIP Caster port" << std::endl;
    std::cerr << "    -m,  --mount MOUNTPOINT       NTRIP mount point" << std::endl;
    std::cerr << "    -u,  --user USERNAME          NTRIP user name" << std::endl;
    std::cerr << "    -pw, --password PASSWORD      NTRIP password" << std::endl;
    std::cerr << "    -la, --latitude LATITUDE      GNSS base station reference latitude" << std::endl;
    std::cerr << "    -lo, --longitude LONGITUDE    GNSS base station reference longitude" << std::endl;
    return 1;
}

int main(int argc, const char* argv[])
{
    if (argc == 1) {
        return print_usage();
    }
    constexpr double noGeo {std::numeric_limits<double>::max()};
    double latitude{noGeo}, longitude{noGeo};
    std::string name{}, password{}, mount{}, address{};
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
        send_correction(login);
        constexpr int retry_period = 30;
        std::cerr << "ntserver: retrying in " << retry_period << " seconds..." << std::endl;
        sleep(retry_period);
    }

    return 0;
}

void send_correction(VrsTunnel::Ntrip::ntrip_login& login)
{
    VrsTunnel::Ntrip::ntrip_server ns{};
    auto res = ns.connect(login);
    if (res == VrsTunnel::Ntrip::status::authfailure) {
        std::cerr << "ntserver: authentication failure." << std::endl;
        return;
    }
    else if (res == VrsTunnel::Ntrip::status::error) {
        std::cerr << "ntserver: connection error." << std::endl;
        return;
    }
    else if (res == VrsTunnel::Ntrip::status::nomount) {
        std::cerr << "ntserver: mount point not found." << std::endl;
        return;
    }

    for (;;) {
        ssize_t n_read = 0;
        int n_bytes_avail = 0;
        std::unique_ptr<char[]> data;
        int ir = ::ioctl(STDIN_FILENO, FIONREAD, &n_bytes_avail);
        if (ir != 0) {
            std::cerr << "ntserver: read correction error." << std::endl;
            ns.disconnect();
            return;
        }
        if (n_bytes_avail > 0) {
            data = std::make_unique<char[]>(n_bytes_avail);
            n_read = ::read(STDIN_FILENO, data.get(), n_bytes_avail);
            if (n_read > 0) {
                auto send_stat = ns.send_begin(data.get(), n_read);
                if (send_stat != VrsTunnel::Ntrip::status::ready) {
                    std::cerr << "ntserver: send correction error." << std::endl;
                    ns.disconnect();
                    return;
                }
            }
        }
        
        constexpr int timeout = 150; /**< 15 seconds (15 times 100ms) */
        for(int i = 0; i < timeout; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (ns.get_status() == VrsTunnel::Ntrip::status::ready) {
                break;
            }
        }
        if (ns.get_status() != VrsTunnel::Ntrip::status::ready) {
            std::cerr << "ntserver: send correction timeout." << std::endl;
            ns.disconnect();
            return;
        }
        if (n_read > 0) {
            if (n_read != ns.send_end()) {
                std::cerr << "ntserver: send correction error." << std::endl;
                ns.disconnect();
                return;
            }
        }
    }
}