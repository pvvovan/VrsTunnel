#include <string>

#include <CppUTest/TestHarness.h>

#include <nmea.hpp>

TEST_GROUP(NmeaTestGroup)
{
};

TEST(NmeaTestGroup, TestOne)
{
    using namespace VrsTunnel::Ntrip;
    auto res = Nmea::getGGA(location(33.123456789, 53.56, 456.235));
    CHECK_EQUAL("$GPGGA,", res);

    res = Nmea::getGGA(location(33.123456, 53.56, 456.235));
    CHECK_EQUAL("$GPGGA,", res);
};

TEST(NmeaTestGroup, TestChecksum)
{
    using namespace VrsTunnel::Ntrip;
    auto res = Nmea::getChecksum("$GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000");
    CHECK_EQUAL("11", res);

    std::string cs = Nmea::getChecksum("$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031");
    CHECK_EQUAL("11", cs);
};

// $GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E 
// $GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F
