#include <msgpack.hpp>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>

#include "msgs.hpp"
#include "zmq.hpp"
#include "serialization.hpp"
// #include "transport.cpp"
// #include "transport-impl.cpp"

using namespace std;


#include <sstream>
#include <cassert>

#include <msgpack.hpp>

class Vec {
public:
    double x,y,z;
    MSGPACK_DEFINE(x,y,z);
};

int main() {
    Vec v = {1,2,3};
    std::string val = "ABC";
    msgpack::type::ext e1(42, &v, sizeof(Vec));
    msgpack::type::ext ee(43, e1.data(), e1.size());

    assert(e1.type() == 42);
    assert(e1.size() == 3);
    assert(e1.data()[0] == 'A');
    assert(e1.data()[1] == 'B');
    assert(e1.data()[2] == 'C');

    std::stringstream ss;
    msgpack::pack(ss, e1);

    auto oh = msgpack::unpack(ss.str().data(), ss.str().size());
    auto e2 = oh.get().as<msgpack::type::ext>();
    assert(e1 == e2);
}
