
#include <bits/c++config.h>
#include <bits/stdint-uintn.h>
#include <future>
#include <string>
#include <thread>
#include <atomic>
#include <vector>

#include "tcp_server.hpp.cpp"
#include "accept_listener.hpp"

#include <CppUTest/TestHarness.h>
// // #include <CppUTestExt/MockSupport.h>


class accept_listener_test
{
public:
    void OnClientConnected(std::unique_ptr<VrsTunnel::Ntrip::tcp_client> client)
    {
        if (client) {
            --clinet_count;
        }
    };
    std::atomic_int clinet_count{0};
};

namespace VrsTunnel::Ntrip
{
    template bool tcp_server::start(uint16_t port, accept_listener_test& listener);
    template void tcp_server::run_accepting(struct sockaddr* addr, int sockfd, accept_listener_test& listener);
}

constexpr static int tcp_port = 2103;
static accept_listener_test alt{};
static VrsTunnel::Ntrip::tcp_server ts{};
static constexpr int total = 5;

TEST_GROUP(TservTestGroup)
{
    void setup()
    {
        alt.clinet_count = total;
    }
    void teardown()
    {
        ts.stop();
    }
};

TEST(TservTestGroup, ClientCountTest)
{
    bool isOk = ts.start(tcp_port, alt);
    CHECK_TRUE(isOk);

    for(int i = total; i > 0; --i) {
        VrsTunnel::Ntrip::tcp_client tc{};
        auto stat = tc.connect("localhost", tcp_port);
        CHECK_TRUE(stat == VrsTunnel::Ntrip::io_status::Success);
        tc.close();
        while (i != alt.clinet_count + 1) { }
    }
    ts.stop();
    CHECK_EQUAL(0, alt.clinet_count);
    ts.stop();
};


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
    VrsTunnel::Ntrip::accept_listener al{};
    constexpr int size = 111;
    std::vector<std::future<void>> inserters{size};
    std::vector<std::promise<void>> ready_elems;
    std::promise<void> go{};
    std::shared_future<void> ready(go.get_future());
    for (int i = 0; i < size; ++i) {
        ready_elems.emplace_back(std::promise<void>{});
    }
    for (std::size_t i = 0; i < size; ++i) {
        inserters[i] = std::async(std::launch::async,
            [ready, &al, i, &ready_elems] () -> void
            {
                ready_elems.at(i).set_value();
                ready.wait();
                al.OnClientConnected(std::make_unique<VrsTunnel::Ntrip::tcp_client>(-i));
            });
    }
    for (std::size_t i = 0; i < size; ++i) {
        ready_elems.at(i).get_future().wait();
    }
    go.set_value();
    for (std::size_t i = 0; i < size; ++i) {
        inserters.at(i).get();
    }
    CHECK_EQUAL(size, al.get_asyncs().size());
}