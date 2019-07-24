#include "lefe_encoder.hpp"
#include <iostream>

namespace VrsTunnel::Ntrip
{
    using namespace std;

    // lefe_encoder::lefe_encoder() {
    //     // cout << "lefe ctr\n";
    // }

    string lefe_encoder::get(string name, string password) {

        const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

        string combined = name + ":" + password;
        char const* bytes_to_encode = combined.c_str();
        unsigned int in_len = combined.length();
        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; (i <4) ; i++) {
                    ret += base64_chars[char_array_4[i]];
                }
                i = 0;
            }
        }

        if (i) {
            for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

            char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

            for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

            while((i++ < 3)) {
                ret += '=';
            }
        }

        return ret;
    };

    unique_ptr<lefe_encoder> lefe_encoder::make() {
        return make_unique<lefe_encoder>();
    }
}