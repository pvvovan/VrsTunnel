#include <string>

#include <CppUTest/TestHarness.h>

#include "base64_encoder.hpp"

TEST_GROUP(EncoderTestGroup)
{
};

TEST(EncoderTestGroup, TestOne)
{
    auto encoder = VrsTunnel::Ntrip::base64_encoder::make_instance();
    std::string res = encoder->get("Raven4", "Raven4");
    CHECK_EQUAL("UmF2ZW40OlJhdmVuNA==", res);
};

TEST(EncoderTestGroup, TestTwo)
{
    using namespace std;
    using namespace VrsTunnel::Ntrip;
    shared_ptr<login_encode> encoder = base64_encoder::make_instance();
    std::string res = encoder->get("Raven4", "Raven4");
    CHECK_EQUAL("UmF2ZW40OlJhdmVuNA==", res);
};