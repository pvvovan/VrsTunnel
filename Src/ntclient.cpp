#include <iostream>
#include <limits>
#include <iomanip>
#include <algorithm>

#include "cli.hpp"
#include "ntrip_client.hpp"

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
    VrsTunnel::Ntrip::ntrip_client nc{};
    auto res = nc.getMountPoints(address, port, username, password);
    if (std::holds_alternative<VrsTunnel::Ntrip::io_status>(res)) {
        std::cout << "Error retreiving mount points." << std::endl;
    }
    else {
        auto mounts = std::get<std::vector<VrsTunnel::Ntrip::mount_point>>(res);
        using mp = VrsTunnel::Ntrip::mount_point;
        auto maxMount = std::max_element(mounts.begin(), mounts.end(), 
            [](const mp& a, const mp& b) { return a.name.size() < b.name.size(); });
        std::size_t maxCorr = 0;
        auto setMax = [&maxCorr](const mp& m) {
            std::size_t l = m.type.size();
            if (l > maxCorr) {
                maxCorr = l;
            }
        };
        std::for_each(mounts.begin(), mounts.end(), setMax);
        for(const auto& m : mounts) {
            std::cout << std::setw(maxMount->name.size()) << std::left << m.name << "\t[" << std::setw(maxCorr) 
                << m.type << "\t(" << m.reference.Latitude << "; " << m.reference.Longitude << ")]" << std::endl;
        }
    }
    
    return 0;
}

void output_correction(VrsTunnel::Ntrip::ntrip_login login)
{
    VrsTunnel::Ntrip::ntrip_client nc{};
    auto res = nc.connect(login);
    if (res == VrsTunnel::Ntrip::status::authfailure) {
        std::cerr << "ntclient: authentication failure." << std::endl;
        return;
    }
    else if (res == VrsTunnel::Ntrip::status::error) {
        std::cerr << "ntclient: connection error." << std::endl;
        return;
    }
    else if (res == VrsTunnel::Ntrip::status::nomount) {
        std::cerr << "ntclient: mount point not found." << std::endl;
        return;
    }

    auto sendgga = [&nc, &login]() -> bool { // return error occured
        auto time = std::chrono::system_clock::now();
        auto send_res = nc.send_gga_begin(login.position, time);
        if (send_res != VrsTunnel::Ntrip::io_status::Success) {
            std::cerr << "ntclient: NMEA GGA sending error." << std::endl;
            nc.disconnect();
            return true;
        }
        return false;
    };
    auto gga_timeout = [&sendgga, &nc]() -> bool { // return error occured
        constexpr int timeout = 100; // 10 seconds (100 times 100ms)
        static int tick = 0;
        tick++;
        if (tick < timeout) { // wait for 10 seconds before proceeding
            return false;
        }
        tick = 0;
        if (nc.get_status() != VrsTunnel::Ntrip::status::ready) {
            std::cerr << "ntclient: NMEA GGA send timeout." << std::endl;
            nc.disconnect();
            return true;
        }
        auto n_sent = nc.send_end();
        if (n_sent > 0) {
            return sendgga();
        }
        else {
            std::cerr << "ntclient: NMEA GGA could not be delivered." << std::endl;
            nc.disconnect();
            return true;
        }
    };
    auto status_timeout = [&nc](bool& available) -> bool { // return error occured
        constexpr int timeout = 300; // 30 seconds (300 times 100ms)
        static int tick = 0;
        ++tick;
        if (tick < timeout) { // wait for 30 seconds before proceeding
            return false;
        }
        tick = 0;
        if (!available) { // correction did not arrive in 30 seconds
            std::cerr << "ntclient: no correction available." << std::endl;
            nc.disconnect();
            return true;
        }
        available = false;
        auto stat = nc.get_status();
        switch (stat)
        {
        // ready or sending is expected
        case VrsTunnel::Ntrip::status::ready:
        case VrsTunnel::Ntrip::status::sending:
            return false;
        default: // otherwise return error
            std::cerr << "ntclient: error!" << std::endl;
            nc.disconnect();
            return true;
            break;
        }
    };

    sendgga();
    bool data_available = true;
    for (;;) {
        if (gga_timeout()) {
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int avail = nc.available();
        if (avail > 0) {
            auto corr = nc.receive(avail);
            fwrite(corr.get(), avail, 1, stdout);
            fflush(stdout);
            data_available = true;
        }

        if (status_timeout(data_available)) {
            return;
        }
    }
}

int main(int argc, const char* argv[])
{
    if (argc == 1) {
        return print_usage();
    }

    constexpr double noGeo {std::numeric_limits<double>::max()};
    double latitude{noGeo}, longitude{noGeo};
    std::string username{}, password{}, mount{}, address{}, yesno{};
    int port{0};

    try
    {
        VrsTunnel::cli cli(argc, argv);
        cli.retrieve({"a", "-address"}, address);
        cli.retrieve({"p", "-port"}, port);
        cli.retrieve({"m", "-mount"}, mount);
        cli.retrieve({"u", "-user"}, username);
        cli.retrieve({"pw", "-password"}, password);
        cli.retrieve({"g", "-get"}, yesno);
        cli.retrieve({"la", "-latitude"}, latitude);
        cli.retrieve({"lo", "-longitude"}, longitude);
    }
    catch (const std::bad_variant_access& err)
    {
        return print_usage();
    }
    catch (const std::runtime_error &err)
    {
        std::cerr << " ...err: " << err.what() << std::endl;;
        return print_usage();
    }

    if (yesno.compare("yes") == 0 || yesno.compare("y") == 0) {
        return showMountPoints(address, port, username, password);
    }

    if (latitude == noGeo || longitude == noGeo || port == 0
            || address.size() == 0 || mount.size() == 0
            || username.size() == 0 || password.size() == 0) {
        return print_usage();
    }

    VrsTunnel::Ntrip::ntrip_login login{};
    login.address = address;
    login.port = port;
    login.mountpoint = mount;
    login.username = username;
    login.password = password;
    login.position.Latitude = latitude;
    login.position.Longitude = longitude;
    for (;;) {
        output_correction(login);
        constexpr int retry_period = 30;
        std::cerr << "ntclient: retrying in " << retry_period << " seconds..." << std::endl;
        sleep(retry_period);
    }
    return 0;
}