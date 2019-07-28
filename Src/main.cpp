#include <iostream>
#include <login_encode.hpp>
#include <base64_encoder.hpp>
#include <location.hpp>
#include <nmea.hpp>
#include <chrono>

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



}