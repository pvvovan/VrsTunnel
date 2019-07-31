#include <iostream>
#include <chrono>

#include "login_encode.hpp"
#include "base64_encoder.hpp"
#include "location.hpp"
#include "nmea.hpp"
#include "async_io.hpp"
#include "tcp_client.hpp"
#include "NtripClient.hpp"


int main(int argc, char** argv) {
    using namespace std;
    cout << "OK " << (71^80) << endl;

    // cout << "enc=" << VrsTunnel::Ntrip::lefe_encoder{}.get("n", "p") << endl;
    cout << "enc=" << VrsTunnel::Ntrip::base64_encoder::make_instance()->get("n", "p") << endl;
    constexpr VrsTunnel::Ntrip::location loc {1, 2, 3};
    cout << loc.Elevation << endl;

    // VrsTunnel::Ntrip::Nmea nm{};
    cout << std::get<std::string>(VrsTunnel::Ntrip::nmea::getGGA(VrsTunnel::Ntrip::location(-1.1, 2.2, 3.3), 
            std::chrono::system_clock::now())) << endl;

    // VrsTunnel::Ntrip::tcp_client tcp{};
    // auto res = tcp.Connect("192.168.1.6", 44444);
    // if (res == VrsTunnel::Ntrip::io_status::Success) {
    //     cout << "Connect Success\n";
    //     VrsTunnel::Ntrip::async_io asyio {tcp.get_sockfd()};
    //     res = asyio.Write("VOvan", 5);
    //     if (res == VrsTunnel::Ntrip::io_status::Success) {
    //         while(asyio.Check() != VrsTunnel::Ntrip::io_status::Success);
    //         cout << "Write Success: " << asyio.End() << endl;
    //         while(!asyio.Available());
    //         int avail = asyio.Available();
    //         cout << "avial: " << avail << endl;
    //         auto b = asyio.Read(avail);
    //         string resp(b.get(), avail);
    //         cout << "resp: " << resp << endl;
    //     }
    // }
    // else
    // {
    //     cout << "tcp error\n";
    // }
    
    VrsTunnel::Ntrip::NtripClient nc{};
    auto r = nc.getMountPoints("195.16.76.194", 2101);
    if (std::holds_alternative<std::vector<VrsTunnel::Ntrip::MountPoint>>(r)) {
        cout << std::get<std::vector<VrsTunnel::Ntrip::MountPoint>>(r)[0].Raw << endl;
    }
}