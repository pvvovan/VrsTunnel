#include <gtest/gtest.h>
#include <string>
#include <nmea.hpp>

TEST(testNmea, testChecksum)
{
    using namespace VrsTunnel::Ntrip;
    auto res = nmea::getChecksum("GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000");
    EXPECT_EQ("6E", res);

    std::string cs = nmea::getChecksum("GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031");
    EXPECT_EQ("4F", cs);
}

// $GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E 
// $GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F