#include <gtest/gtest.h>

#include <string>
#include <chrono>
#include <thread>

#include "tcp_client.hpp"
#include "ntrip_client.hpp"



TEST(TcpTestGroup, TestTcpClient)
{
    VrsTunnel::Ntrip::tcp_client tc{};
    auto con_res = tc.connect("195.16.76.194", 2101);
    if (con_res != VrsTunnel::Ntrip::io_status::Success) {
        EXPECT_TRUE(false);
    }
    VrsTunnel::Ntrip::ntrip_client nc{};
    VrsTunnel::Ntrip::async_io aio{tc.get_sockfd()};
    const char* request = "GET / HTTP/1.0\r\n"
        "User-Agent: NTRIP PvvovanNTRIPClient/\r\n"
        "Accept: */*\r\n" "Connection: close\r\n" "\r\n";
    
    auto res = aio.write(request, strlen(request));
    if (res != VrsTunnel::Ntrip::io_status::Success) {
        EXPECT_TRUE(false);
    }
    std::string responseRaw{};
    for(int i = 0; i < 50; i++) { // 5 seconds timeout
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int avail = aio.available();
        if (avail < 0) {
            EXPECT_TRUE(false);
        }
        else if (avail > 0) {
            auto chunk = aio.read((size_t)avail);
            responseRaw.append(chunk.get(), (size_t)avail);
            if (nc.hasTableEnding(responseRaw)) {
                break;
            }
        }
    }
    if (!nc.hasTableEnding(responseRaw)) {
        EXPECT_TRUE(false);
    }
    tc.close();
    EXPECT_EQ(responseRaw.size(), 1461UL);
    tc.close();
};
