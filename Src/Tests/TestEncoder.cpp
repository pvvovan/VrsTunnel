#include <string>

#include <CppUTest/TestHarness.h>

#include "lefe_encoder.hpp"

TEST_GROUP(EncoderTestGroup)
{
};

TEST(EncoderTestGroup, TestOne)
{
    auto encoder = VrsTunnel::Ntrip::lefe_encoder::make();
    std::string res = encoder->get("Raven4", "Raven4");
    CHECK_EQUAL("UmF2ZW40OlJhdmVuNA==", res);
};