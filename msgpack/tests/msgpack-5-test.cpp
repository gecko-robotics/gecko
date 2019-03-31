#include <msgpack.hpp>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>

#include "msgs.hpp"
#include "zmq.hpp"
#include "serialization.hpp"
// #include "MsgPack.cpp"
// #include "MsgPack-impl.cpp"

using namespace std;


int main(){
    vec_t b(1,2,3), c(4,5,6), d(-1,-2,-3);
    imu_t a(b,c,d);
    a.print();

    MsgPack<imu_t> buffer;
    // buffer.compression_level = Z_BEST_SPEED;  // default
    // buffer.compression_level = Z_BEST_COMPRESSION;
    // buffer.compression_level = Z_DEFAULT_COMPRESSION;
    zmq::message_t m = buffer.pack(a);

    cout << "main message_t size: " << m.size() << endl;
    cout << "main addr: " << &m << endl;
    cout << "message: " << m << endl;

    // vector<imu_t> resp;
    // buffer.unpack(m, resp);
    //
    // for (const imu_t& i: resp) {
    //     i.print();
    //     assert(a == i);
    // }

    return 0;
}
