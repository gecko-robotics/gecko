#include <msgpack.hpp>
#include <msgpack/zbuffer.hpp>
#include <stdio.h>
#include <ctime>
#include <sys/time.h>
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

enum GeckoMsgs {
    GVECTOR,
    GQUATERNION,
    GIMU,
    GWRENCH,
    GTWIST,
    GLIDAR,
    GIMAGE,
    GPOSE
};

/**
 * Base class for keeping track of message type
 */
class base_t {
public:
    base_t(uint8_t t): type(t) {}
    uint8_t type;
    MSGPACK_DEFINE(type);
};

class vec_t: public base_t {
public:
    vec_t(): base_t(GVECTOR) {}
    vec_t(vec_t& a): base_t(GVECTOR) {
        x = a.x;
        y = a.y;
        z = a.z;
    }
    vec_t(double a, double b, double c): base_t(GVECTOR){
        x = a;
        y = b;
        z = c;
    }

    double x,y,z;

    MSGPACK_DEFINE(MSGPACK_BASE(base_t),x,y,z);

    void print(char end='\n'){
        printf(" [%f %f %f]%c", x, y, z,end);
    }
};

class msg_t {
public:
    msg_t(): timestamp(time_now()) {}
    double time_now(){
        struct timeval ts;
        gettimeofday(&ts, NULL);
        return ((double)(ts.tv_sec) + 1e-6*(ts.tv_usec));
    }
    double timestamp;

    MSGPACK_DEFINE(timestamp);
};

class imu_t: public base_t, public msg_t  {
public:
    imu_t(): base_t(GIMU) {}
    imu_t(vec_t a, vec_t g, vec_t m): base_t(GIMU), accel(a), gyro(g), mag(m) {}
    vec_t accel, gyro, mag;

    MSGPACK_DEFINE(MSGPACK_BASE(base_t),accel,gyro,mag,MSGPACK_BASE(msg_t));

    void print(){
        printf("IMU [%f]\n", timestamp);
        accel.print();
        gyro.print();
        mag.print();
    }
};

int main(){
    // test a = {1,3,5,"hello"};
    // printf("a: %d %d %d %s\n", a.a, a.b, a.c, a.d.c_str());

    vec_t b(1,2,3), c(4,5,6), d(-1,-2,-3);
    imu_t a(b,c,d);
    a.print();

    // stringstream ss;
    msgpack::zbuffer ss;
    msgpack::pack(ss, a);
    size_t offset = 0;
    cout << "offset: " << offset << endl;
    // cout << "packed size: " << ss.str().size() << endl;
    // {
    //     char c[500];
    //     z_stream infstream;
    //     std::string decompressed;
    //
    //     infstream.zalloc = Z_NULL;
    //     infstream.zfree = Z_NULL;
    //     infstream.opaque = Z_NULL;
    //     // setup "b" as the input and "c" as the compressed output
    //     infstream.avail_in = (uInt)((char*)defstream.next_out - b); // size of input
    //     infstream.next_in = (Bytef *)ss.data(); // input char array
    //     infstream.avail_out = (uInt)sizeof(c); // size of output
    //     infstream.next_out = (Bytef *)c; // output char array
    //
    //     // the actual DE-compression work.
    //     inflateInit(&infstream);
    //     inflate(&infstream, Z_NO_FLUSH);
    //     inflateEnd(&infstream);
    //
    //     msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
    //     // msgpack::object_handle oh = msgpack::unpack(ss.data(), ss.size(), offset);
    //     msgpack::object obj = oh.get();
    //
    //     cout << obj << endl;
    //
    //     imu_t e;
    //     obj.convert(e);
    //     e.print();
    //
    //     // assert(a.a == b.a);
    //     // assert(a.b == b.b);
    //     // assert(a.c == b.c);
    //     // assert(a.d == b.d);
    // }

    return 0;
}
