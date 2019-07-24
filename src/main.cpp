#include <iostream>
#include <login_encode.hpp>
#include <lefe_encoder.hpp>

int main(int argc, char** argv) {
    using namespace std;
    cout << "OK\n";

    // cout << "enc=" << VrsTunnel::Ntrip::lefe_encoder{}.get("n", "p") << endl;
    cout << "enc=" << VrsTunnel::Ntrip::lefe_encoder::make()->get("n", "p") << endl;
    
}