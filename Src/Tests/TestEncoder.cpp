#include <string>

#include <CppUTest/TestHarness.h>

#include "login_encode.hpp"

TEST_GROUP(EncoderTestGroup)
{

};

TEST(EncoderTestGroup, TestOne)
{
    auto encoder = VrsTunnel::Ntrip::login_encode::make_instance();
    std::string res = encoder->get("Raven4", "Raven4");
    CHECK_EQUAL("UmF2ZW40OlJhdmVuNA==", res);
}

TEST(EncoderTestGroup, TestTwo)
{
    using namespace std;
    using namespace VrsTunnel::Ntrip;
    shared_ptr<login_encode> encoder = login_encode::make_instance();
    std::string res = encoder->get("Raven1", "Raven1");
    CHECK_EQUAL("UmF2ZW4xOlJhdmVuMQ==", res);
}
