#include <msgpack.hpp>
#include <msgpack/zbuffer.hpp>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>

using namespace std;

class base1 {
public:
   int a;
   MSGPACK_DEFINE(a);
};
class base2 {
public:
   int b;
   MSGPACK_DEFINE(b);
};
class test: public base1, public base2 {
public:
    int c;
    std::string d;
    // You can choose any order. It is represented to the msgpack array elements order.
    MSGPACK_DEFINE(d, MSGPACK_BASE(base2), c, MSGPACK_BASE(base1));
};

int main(){
    test a = {1,3,5,"hello"};
    printf("a: %d %d %d %s\n", a.a, a.b, a.c, a.d.c_str());

    stringstream ss;
    // msgpack::zbuffer ss;
    msgpack::pack(ss, a);
    size_t offset = 0;
    cout << "offset: " << offset << endl;
    cout << "packed size: " << ss.str().size() << endl;
    // cout << "packed size: " << ss.size() << endl;
    {
        msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
        // msgpack::object_handle oh = msgpack::unpack(ss.data(), ss.size(), offset);
        // std::string decompressed = deflate(&ss.data(), Z_FINISH)
        // msgpack::object_handle oh = msgpack::unpack(decompressed.data(), decompressed.size(), offset);
        msgpack::object obj = oh.get();

        cout << obj << endl;

        test b;
        obj.convert(b);

        assert(a.a == b.a);
        assert(a.b == b.b);
        assert(a.c == b.c);
        assert(a.d == b.d);
    }

    return 0;
}
