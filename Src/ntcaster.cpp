#include <iostream>
#include <chrono>
#include <functional>

#include "tcp_server.hpp"
#include "tcp_client.hpp"


static void client_connected(VrsTunnel::Ntrip::async_io client)
{
	auto res = client.write("hello\r\n", 7);
	std::cout << "Client status = " << static_cast<int>(res) << std::endl;
	::sleep(1);
	std::cout << "Sent " << client.end() << " bytes" << std::endl;
	client.close();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	std::function<void(VrsTunnel::Ntrip::async_io)> cl_con {client_connected};
	VrsTunnel::Ntrip::tcp_server ts{};
	if (ts.start(8023, cl_con) == false ) {
		std::cerr << "Failed to start server" << std::endl;
	} else {
		::sleep(1);
		VrsTunnel::Ntrip::tcp_client tc{-1};
		(void)tc.connect("127.0.0.1", 8023);
		::sleep(1);
		VrsTunnel::Ntrip::io_status stat = tc.connect("127.0.0.1", 8023);
		(void)stat;
		::sleep(5);
		ts.stop();
	}
}
