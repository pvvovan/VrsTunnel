import cli;

#include <iostream>
#include <vector>

#include "ntrip_server.hpp"


static void send_correction(VrsTunnel::Ntrip::ntrip_login& login)
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
			data = std::make_unique<char[]>((size_t)n_bytes_avail);
			n_read = ::read(STDIN_FILENO, data.get(), (size_t)n_bytes_avail);
			if (n_read > 0) {
				auto send_stat = ns.send_begin(data.get(), (size_t)n_read);
				if (send_stat != VrsTunnel::Ntrip::status::ready) {
					std::cerr << "ntserver: send correction error." << std::endl;
					ns.disconnect();
					return;
				}
			}
		}
		// wait for the connection to become ready
		constexpr int timeout = 150; /* 15 seconds (15 times 100ms) */
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
			if (n_read != ns.send_end()) { // not all the data have been transmitted
				std::cerr << "ntserver: deliver correction error." << std::endl;
				ns.disconnect();
				return;
			}
		}
	}
}

static int print_usage()
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
	std::string username{}, password{}, mount{}, address{};
	int port{0};
	try
	{
		VrsTunnel::cli cli(argc, argv);
		cli.retrieve({"a", "-address"}, address);
		cli.retrieve({"p", "-port"}, port);
		cli.retrieve({"m", "-mount"}, mount);
		cli.retrieve({"u", "-user"}, username);
		cli.retrieve({"pw", "-password"}, password);
		cli.retrieve({"la", "-latitude"}, latitude);
		cli.retrieve({"lo", "-longitude"}, longitude);
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
			|| username.size() == 0 || password.size() == 0) {
		return ::print_usage();
	}

	VrsTunnel::Ntrip::ntrip_login login{};
	login.address = address;
	login.port = static_cast<unsigned short>(port);
	login.mountpoint = mount;
	login.username = username;
	login.password = password;
	login.position.Latitude = latitude;
	login.position.Longitude = longitude;
	for (;;) {
		::send_correction(login);
		constexpr int retry_period = 30;
		std::cerr << "ntserver: retrying in " << retry_period << " seconds..." << std::endl;
		::sleep(retry_period);
	}

	return 0;
}
