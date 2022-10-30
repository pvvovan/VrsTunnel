#include <iostream>
#include <chrono>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <unistd.h>

#include "dispatcher.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	VrsTunnel::Ntrip::dispatcher dispatch{};

	std::set<std::string> srv_auth{};
	srv_auth.insert("srvpathw");

	std::map<std::string, std::set<std::string>> cli_auth{};
	cli_auth["srvpathw"].insert("clipassw");

	if (dispatch.start(8021, cli_auth, 8025, srv_auth) == false ) {
		std::cerr << "Failed to start dispatcher" << std::endl;
	} else {
		::sleep(1);
		::sleep(1);
		::sleep(5);
		dispatch.stop();
	}
}
