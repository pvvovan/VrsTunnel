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

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    VrsTunnel::Ntrip::ntrip_server ntserver{};
    VrsTunnel::Ntrip::ntrip_login ntl{};
    ntl.address = "titanmachinery.ua";
    ntl.port = 8023;
    ntl.position = VrsTunnel::Ntrip::location(51, 31, 0);
    ntl.username = "test";
    ntl.password = "test";
    ntl.mountpoint = "myMout";
    auto scn = ntserver.connect(ntl);
    if (scn == VrsTunnel::Ntrip::status::ready) {
        std::cout << "server ready\n";
        for (;;) {
            auto printErr = [](VrsTunnel::Ntrip::status s) { 
                if (s != VrsTunnel::Ntrip::status::ready) {
                    std::cerr << "nserver: send error\n";
                }
                else {
                    std::cerr << "nserver: OK\n";
                }
            };
            printErr(ntserver.get_status());
            sleep(1);
            printErr(ntserver.send_begin("1234567890", 10));
            sleep(1);
            if (ntserver.send_end() != 10) {
                std::cerr << "nserver: send error\n";
            }
        }
    }

    using namespace std;
    cout << "OK " << (71^80) << endl;
    std::string strnum{"123.123"};
    std::size_t pos = 0;
    cout << std::stod(strnum, &pos) << endl;
    cout << pos << "-" << strnum.length() << endl;

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
    
    VrsTunnel::Ntrip::ntrip_client nc{};
    auto r = nc.getMountPoints("195.16.76.194", 2101);
    if (std::holds_alternative<std::vector<VrsTunnel::Ntrip::mount_point>>(r)) {
        cout << std::get<std::vector<VrsTunnel::Ntrip::mount_point>>(r)[0].raw_entry << endl;
    }

    VrsTunnel::Ntrip::ntrip_client ntclient{};
    std::string ntaddress = "titanmachinery.ua"; // "194.28.183.167"; "94.153.224.194"
    int ntport = 8021;
    auto mps = ntclient.getMountPoints(ntaddress, ntport);
    VrsTunnel::Ntrip::ntrip_login ntlogin{};
    ntlogin.address = ntaddress;
    ntlogin.port = ntport;
    ntlogin.position = VrsTunnel::Ntrip::location(51, 31, 0);
    ntlogin.username = "test";
    ntlogin.password = "test";
    ntlogin.mountpoint = std::get<std::vector<VrsTunnel::Ntrip::mount_point>>(mps)[0].name;
    cout << ntlogin.mountpoint << endl;
    auto con_res = ntclient.connect(ntlogin);
    if (con_res == VrsTunnel::Ntrip::status::error) {
        cerr << "could not connect\n";
        return 1;
    }
    else if (con_res == VrsTunnel::Ntrip::status::authfailure) {
        cerr << "authentication failure\n";
        return 1;
    }

    int counter = 9;
    for(;;) {
        ++counter;
        if (counter == 10) {
            counter = 0;
            auto send_res = ntclient.send_gga(VrsTunnel::Ntrip::location(50, 30, 0),
                    std::chrono::system_clock::now());
            if (send_res != VrsTunnel::Ntrip::io_status::Success) {
                cerr << "gga sending error\n";
            }
        }

        sleep(1);
        int avail = ntclient.available();
        if (avail > 0) {
            auto corr = ntclient.receive(avail);
            fwrite(corr.get(), avail, 1, stdout);
        }
        else {
            cerr << "nothing available\n";
        }
    }
}