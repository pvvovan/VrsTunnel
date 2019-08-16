#include <iostream>
#include <chrono>

#include "login_encode.hpp"
#include "base64_encoder.hpp"
#include "location.hpp"
#include "nmea.hpp"
#include "async_io.hpp"
#include "tcp_client.hpp"
#include "ntrip_client.hpp"
#include "ntrip_server.hpp"
#include "tcp_server.hpp"
#include "accept_listener.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    VrsTunnel::Ntrip::tcp_server ts{};
    VrsTunnel::Ntrip::accept_listener al;
    [[maybe_unused]] bool r = ts.start(8023, al);
    sleep(1);
    ts.stop();
}