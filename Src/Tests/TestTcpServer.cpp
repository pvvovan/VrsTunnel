
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
    template bool tcp_server::start(int, accept_listener_test&);
    template void tcp_server::run_accepting(struct sockaddr*, int, accept_listener_test&);
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
    std::vector<std::future<void>> inserters{};
    std::vector<std::promise<void>> ready_elems{};
    std::promise<void> go{};
    std::shared_future<void> ready(go.get_future());
    for (int i = 0; i < size; ++i) {
        ready_elems.emplace_back(std::promise<void>{});
    }
    for (int i = 0; i < size; ++i) {
        inserters.emplace_back(std::async(std::launch::async,
        [ready, &al, i, &ready_elems]
        {
            ready_elems[i].set_value();
            ready.wait();
            al.OnClientConnected(std::make_unique<VrsTunnel::Ntrip::tcp_client>(-i));
        }));
    }
    for (int i = 0; i < size; ++i) {
        ready_elems[i].get_future().wait();
    }
    go.set_value();
    for (int i = 0; i < size; ++i) {
        inserters[i].get();
    }
    CHECK_EQUAL(size, al.get_asyncs().size());
}