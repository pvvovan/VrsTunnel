#include <gtest/gtest.h>

#include <string>
#include <chrono>
#include <thread>

#include "tcp_client.hpp"
#include "NtripClient.hpp"


TEST(TcpTestGroup, TestTcpClient)
{
    VrsTunnel::Ntrip::tcp_client tc{};
    tc.Connect("195.16.76.194", 2101);
    VrsTunnel::Ntrip::NtripClient nc{};
    VrsTunnel::Ntrip::async_io aio{tc.get_sockfd()};
    const char* request = "GET / HTTP/1.0\r\n"
        "User-Agent: NTRIP PvvovanNTRIPClient/\r\n"
        "Accept: */*\r\n" "Connection: close\r\n" "\r\n";
    
    auto res = aio.Write(request, strlen(request));
    if (res != VrsTunnel::Ntrip::io_status::Success) {
        EXPECT_TRUE(false);
    }
    std::string responseRaw{};
    for(int i = 0; i < 50; i++) { // 5 seconds timeout
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto avail = aio.Available();
        if (avail < 0) {
            EXPECT_TRUE(false);
        }
        else if (avail > 0) {
            auto chunk = aio.Read(avail);
            responseRaw.append(chunk.get(), avail);
            if (nc.hasTableEnding(responseRaw)) {
                break;
            }
        }
    }
    if (!nc.hasTableEnding(responseRaw)) {
        EXPECT_TRUE(false);
    }
    tc.Close();
    EXPECT_EQ(responseRaw.size(), 853);
    tc.Close();
};

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
