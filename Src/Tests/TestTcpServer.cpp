#include <bits/c++config.h>
#include <bits/stdint-uintn.h>
#include <future>
#include <string>
#include <thread>
#include <atomic>
#include <vector>

#include "tcp_server.hpp"
#include "tcp_client.hpp"

#include <CppUTest/TestHarness.h>


constexpr static int tcp_port = 2103;
static void on_client_connected(VrsTunnel::Ntrip::async_io client)
{
    static_cast<void>(client);
}
static VrsTunnel::Ntrip::tcp_server ts{};
// static constexpr int total = 5;

TEST_GROUP(TservTestGroup)
{
    void setup()
    {
        // ts.start(tcp_port, on_client_connected);
    }
    void teardown()
    {
        // ts.stop();
    }
};


TEST(TservTestGroup, ClientCountTest)
{
    CHECK_TRUE(ts.start(tcp_port, on_client_connected));

    for(int i = 5; i > 0; --i) {
        VrsTunnel::Ntrip::tcp_client tc{-1};
        VrsTunnel::Ntrip::io_status stat = tc.connect("127.0.0.1", tcp_port);
        CHECK_TRUE(stat == VrsTunnel::Ntrip::io_status::Error);
        tc.close();
        // while (i != alt.clinet_count + 1) { }
    }
    // ts.stop();
    // CHECK_EQUAL(0, alt.clinet_count);
    ts.stop();
}


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
    // VrsTunnel::Ntrip::accept_listener al{};
    // constexpr int size = 111;
    // std::vector<std::future<void>> inserters{size};
    // std::vector<std::promise<void>> ready_elems;
    // std::promise<void> go{};
    // std::shared_future<void> ready(go.get_future());
    // for (int i = 0; i < size; ++i) {
    //     ready_elems.emplace_back(std::promise<void>{});
    // }
    // for (std::size_t i = 0; i < size; ++i) {
    //     inserters[i] = std::async(std::launch::async,
    //         [ready, &al, i, &ready_elems] () -> void
    //         {
    //             ready_elems.at(i).set_value();
    //             ready.wait();
    //             al.OnClientConnected(std::make_unique<VrsTunnel::Ntrip::tcp_client>(-i));
    //         });
    // }
    // for (std::size_t i = 0; i < size; ++i) {
    //     ready_elems.at(i).get_future().wait();
    // }
    // go.set_value();
    // for (std::size_t i = 0; i < size; ++i) {
    //     inserters.at(i).get();
    // }
    // CHECK_EQUAL(size, al.get_asyncs().size());
}
