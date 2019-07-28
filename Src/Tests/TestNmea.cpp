#include <string>
#include <chrono>

#include <CppUTest/TestHarness.h>

// #include <nmea.hpp>

TEST_GROUP(NmeaTestGroup)
{
};

// TEST(NmeaTestGroup, TestOne)
// {
//     // using namespace VrsTunnel::Ntrip;
//     std::chrono::system_clock::time_point time{};
//     time += std::chrono::seconds(33);
//     time += std::chrono::minutes(22);
//     time += std::chrono::hours(11);
//     // auto res = nmea::getGGA(location(33.123456789, 53.56, 456.235), time);
//     // CHECK_EQUAL("$GPGGA,112233.00,3307.407407,N,05333.600000,E,", std::get<std::string>(res));
// };

// TEST(NmeaTestGroup, TestTwo)
// {
//     using namespace VrsTunnel::Ntrip;
//     std::chrono::system_clock::time_point time{};
//     time += std::chrono::seconds(3);
//     time += std::chrono::minutes(2);
//     time += std::chrono::hours(1);    
//     auto res = nmea::getGGA(location(1.213456, 53.56, 456.235), time);
//     std::string exr {"$GPGGA,010203.00,0112.807360,N,05333.600000,E,"};
//     CHECK_EQUAL(exr, std::get<std::string>(res));
// };

// TEST(NmeaTestGroup, TestThree)
// {
//     using namespace VrsTunnel::Ntrip;
//     std::chrono::system_clock::time_point time{};
//     time += std::chrono::milliseconds(159);
//     time += std::chrono::seconds(59);
//     time += std::chrono::minutes(24);
//     time += std::chrono::hours(3);    
//     std::string res = std::get<std::string>(nmea::getGGA(location(1.821, 53.56, 456.235), time));
//     std::string exr {"$GPGGA,032459.15,0149.260000,N,05333.600000,E,"};
//     CHECK_EQUAL(exr, res);
// };

// TEST(NmeaTestGroup, TestFour)
// {
//     using namespace VrsTunnel::Ntrip;
//     std::chrono::system_clock::time_point time{};
//     time += std::chrono::milliseconds(691);
//     time += std::chrono::seconds(59);
//     time += std::chrono::minutes(24);
//     time += std::chrono::hours(3);    
//     std::string res = std::get<std::string>(nmea::getGGA(location(-1.821, -179.56654789, 456.235), time));
//     std::string exr {"$GPGGA,032459.69,0149.260000,S,17933.992873,W,"};
//     CHECK_EQUAL(exr, res);
// };

// TEST(NmeaTestGroup, TestChecksum)
// {
//     using namespace VrsTunnel::Ntrip;
//     auto res = nmea::checksum("GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000");
//     CHECK_EQUAL(0x6EU, res);

//     uint8_t cs = nmea::checksum("GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031");
//     CHECK_EQUAL(0x4FU, cs);
// };

// $GPGGA,115739.00,4158.8441367,N,09147.4416929,W,4,13,0.9,255.747,M,-32.00,M,01,0000*6E 
// $GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F
