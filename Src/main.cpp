#include <iostream>
#include <login_encode.hpp>
#include <base64_encoder.hpp>
#include <location.hpp>
#include <nmea.hpp>

int main(int argc, char** argv) {
    using namespace std;
    cout << "OK " << (71^80) << endl;

    // cout << "enc=" << VrsTunnel::Ntrip::lefe_encoder{}.get("n", "p") << endl;
    cout << "enc=" << VrsTunnel::Ntrip::base64_encoder::make_instance()->get("n", "p") << endl;
    constexpr VrsTunnel::Ntrip::location loc {1, 2, 3};
    cout << loc.Elevation << endl;

    // VrsTunnel::Ntrip::Nmea nm{};
    cout << VrsTunnel::Ntrip::nmea::getGGA(VrsTunnel::Ntrip::location()) << endl;
    cout << VrsTunnel::Ntrip::nmea::getChecksum("GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031") << endl;
}