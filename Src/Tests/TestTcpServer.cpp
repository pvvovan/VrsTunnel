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


TEST_GROUP(TservTestGroup)
{
void setup()
{

}
void teardown()
{

}
};


TEST(TservTestGroup, ClientCountTest)
{
	
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
