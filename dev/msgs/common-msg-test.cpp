#include <gecko/msgpack/serialization.hpp>
#include <string>
#include <iostream>
#include <gecko/msgpack/msgs.hpp>
// #include <gecko/msgpack/msgs.hpp>

using namespace std;

int main(){
    vec_t a(1,2,3);

    // MsgPack<a_t> packer;
    // zmq::message_t msg = packer.pack(a);
    zmq::message_t msg = a.pack();
    // a_t b = packer.unpack(msg);
    vec_t b(msg);

    a.print();
    b.print();

    cout << (a == b) << endl;

    // --------------------------------------
    quaternion_t c(1,2,3,4);
    zmq::message_t msg2 = c.pack();
    quaternion_t d(msg2);

    c.print();
    d.print();
    cout << (c == d) << endl;

    return 0;
}
