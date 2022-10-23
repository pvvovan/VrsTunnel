#include <iostream>
#include <chrono>
#include <functional>

#include "tcp_server.hpp"


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
	VrsTunnel::Ntrip::tcp_server ts{};
	if (ts.start(8023, client_connected) == false ) {
		std::cerr << "Failed to start server" << std::endl;
	} else {
		::sleep(5);
		ts.stop();
	}
}
