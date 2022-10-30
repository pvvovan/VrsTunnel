#include <bits/c++config.h>
#include <bits/stdint-uintn.h>
#include <future>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <set>

#include "tcp_server.hpp"
#include "tcp_client.hpp"

#include <CppUTest/TestHarness.h>


constexpr static int tcp_port {2103};
static std::atomic<int> client_count {0};
static void on_client_connected(VrsTunnel::Ntrip::async_io client)
{
	static_cast<void>(client);
	client_count++;
}
static VrsTunnel::Ntrip::tcp_server ts{};

TEST_GROUP(TservTestGroup)
{
void setup()
{
	// ts.start(tcp_port, on_client_connected);
}
void teardown()
{
	// ts.stop();
}
};


TEST(TservTestGroup, ClientCountTest)
{
	std::function<void(VrsTunnel::Ntrip::async_io)> cl_con {on_client_connected};
	CHECK_TRUE(ts.start(tcp_port, cl_con));
	client_count = 0;
	int constexpr CL_CNT{5};
	for(int i = CL_CNT; i > 0; --i) {
		VrsTunnel::Ntrip::tcp_client tc{-1};
		VrsTunnel::Ntrip::io_status stat = tc.connect("localhost", tcp_port);
		CHECK_TRUE(stat == VrsTunnel::Ntrip::io_status::Success);
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(100ms);
		tc.close();
		std::this_thread::sleep_for(100ms);
	}
	ts.stop();
	CHECK_EQUAL(CL_CNT, client_count);
}


TEST_GROUP(ServThTestGroup)
{
void setup()
{

}
void teardown()
{

}
};

TEST(ServThTestGroup, ThVecTest)
{
	std::set<std::string> auth{};
	if (auth.find("d") != auth.end()) {
		FAIL("found");
	}

	auth.insert("pass");
	if (auth.find("pass") == auth.end()) {
		FAIL("not found");
	}
}
