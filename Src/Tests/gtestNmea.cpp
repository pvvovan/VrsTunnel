#include <gtest/gtest.h>
#include <string>

#include "nmea.hpp"

TEST(testNmea, testChecksum)
{
    using namespace VrsTunnel::Ntrip;
    auto res = nmea::checksum("GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000");
    EXPECT_EQ(0x6eU, res);

    uint8_t cs = nmea::checksum("GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031");
    EXPECT_EQ(0x4fU, cs);
}

TEST(testNmea, TestGGA_1)
{
    using namespace VrsTunnel::Ntrip;
    std::chrono::system_clock::time_point time{};
    time += std::chrono::seconds(33);
    time += std::chrono::minutes(22);
    time += std::chrono::hours(11);
    auto res = nmea::getGGA(location(33.123456789, 53.56, 456.235), time);
    EXPECT_EQ("$GPGGA,112233.00,3307.40740734,N,05333.60000000,E,4,12,0.9,456.235,M,456.235,M,0,0000*63\r\n", std::get<std::string>(res));
}

TEST(testNmea, TestGGA_2)
{
    using namespace VrsTunnel::Ntrip;
    std::chrono::system_clock::time_point time{};
    time += std::chrono::seconds(3);
    time += std::chrono::minutes(2);
    time += std::chrono::hours(1);
    auto res = nmea::getGGA(location(1.213456, 53.56, 456.235), time);
    std::string exp {"$GPGGA,010203.00,0112.80736000,N,05333.60000000,E,4,12,0.9,456.235,M,456.235,M,0,0000*6B\r\n"};
    EXPECT_EQ(exp, std::get<std::string>(res));
}

TEST(testNmea, TestGGA_3)
{
    using namespace VrsTunnel::Ntrip;
    std::chrono::system_clock::time_point time{};
    time += std::chrono::milliseconds(159);
    time += std::chrono::seconds(59);
    time += std::chrono::minutes(24);
    time += std::chrono::hours(3);
    std::string res = std::get<std::string>(nmea::getGGA(location(1.821, 53.56, 46.5), time));
    std::string exp {"$GPGGA,032459.15,0149.26000000,N,05333.60000000,E,4,12,0.9,46.500,M,46.500,M,0,0000*66\r\n"};
    EXPECT_EQ(exp, res);
}

TEST(testNmea, TestGGA_4)
{
    using namespace VrsTunnel::Ntrip;
    std::chrono::system_clock::time_point time{};
    time += std::chrono::milliseconds(691);
    time += std::chrono::seconds(59);
    time += std::chrono::minutes(24);
    time += std::chrono::hours(3 + 24*9);
    std::string res = std::get<std::string>(nmea::getGGA(location(-1.821, -179.56654789, 456.23), time));
    std::string exr {"$GPGGA,032459.69,0149.26000000,S,17933.99287340,W,4,12,0.9,456.230,M,456.230,M,0,0000*67\r\n"};
    EXPECT_EQ(exr, res);
}


TEST(testNmea, TestGGA_5)
{
    using namespace VrsTunnel::Ntrip;
    std::chrono::system_clock::time_point time{};
    time += std::chrono::milliseconds(0);
    time += std::chrono::seconds(39);
    time += std::chrono::minutes(57);
    time += std::chrono::hours(11);
    location loc{41.980735612, -91.790694882, 255.74749};
    std::string res = std::get<std::string>(nmea::getGGA(loc, time));
    std::string exp {"$GPGGA,115739.00,4158.84413672,N,09147.44169292,W,4,12,0.9,255.747,M,255.747,M,0,0000*74\r\n"};
    EXPECT_EQ(exp, res);
}

// $GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E 
// $GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F
