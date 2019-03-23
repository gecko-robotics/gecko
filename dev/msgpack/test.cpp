#include <stdio.h>
#include <stdint.h>
#include <msgpack.hpp>
#include <string>
#include <assert.h>
#include <sstream>
#include <iostream>
#include "zmq.hpp"
#include "zmq_addon.hpp"

using namespace std;


class vec_t {
public:
    vec_t(): type(0) {}
    vec_t(const vec_t& a): type(0) {
        x = a.x;
        y = a.y;
        z = a.z;
    }
    vec_t(double a, double b, double c): type(0){
        x = a;
        y = b;
        z = c;
    }
    bool operator==(const vec_t& v) const {
        if((x == v.x) && (y == v.y) && (z == v.z) && (type == v.type)) return true;
        return false;
    }
    void print(char end='\n') const {
        printf(" [%f %f %f]%c", x, y, z,end);
    }

    double x,y,z;
    int type;
    // MSGPACK_DEFINE(MSGPACK_BASE(base_t),x,y,z);
    MSGPACK_DEFINE(x,y,z);
};

class imu_t  {
public:
    imu_t(): type(10), timestamp(123.456) {}
    imu_t(const imu_t& i): type(10), timestamp(i.timestamp), accel(i.accel), gyro(i.gyro), mag(i.mag) {}
    imu_t(const vec_t& a, const vec_t& g, const vec_t& m): type(10), timestamp(123.456), accel(a), gyro(g), mag(m) {}
    vec_t accel, gyro, mag;
    double timestamp;
    int type;

    void print() const {
        printf("IMU [%f]\n", timestamp);
        accel.print();
        gyro.print();
        mag.print();
    }

    bool operator==(const imu_t& v) const {
        if((accel == v.accel) && (gyro == v.gyro) && (mag == v.mag) && (type == v.type)) return true;
        return false;
    }

    MSGPACK_DEFINE(accel, gyro, mag, timestamp);
};


template<class msg>
class MsgPack {
public:
    // MsgPack() {}

    zmq::message_t pack(const msg& m);
    // void pack_ext(const msg& m){}
    msg unpack(const zmq::message_t& zm);
    // msg unpack(const std::stringstream& ss);
};


template<class msg>
zmq::message_t MsgPack<msg>::pack(const msg& m){
    std::stringstream ssmsg;
    msgpack::pack(ssmsg, m);

    // std::cout << "packed msg: " << sss.str().size() << " " << sss.str() << std::endl;

    msgpack::type::ext e1(m.type, ssmsg.str().data(), ssmsg.str().size());

    printf(">> packed: %d\n", m.type);

    std::stringstream ss;
    msgpack::pack(ss, e1);

    // std::cout << "packed ext: " << ss.str().size() << " " << ss.str() << std::endl;

    // std::string cs = compress_string(ss.str());
    // std::cout << "compressed string size: " << cs.size() << std::endl;

    zmq::message_t zm(static_cast<void*>(ss.str().data()), ss.str().size());
    // zmq::message_t zm(reinterpret_cast<void*>(cs.data()), cs.size());
    // std::cout << "compressed message_t size: " << zm.size() << std::endl;
    // std::cout << "pack addr: " << &zm << std::endl;
    return zm;
}

template<class msg>
msg MsgPack<msg>::unpack(const zmq::message_t& zm){
    // std::string cs(reinterpret_cast<const char*>(zm.data()), zm.size());
    // std::stringstream ss(cs);

    msg message;
    try {
        // msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size());
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(zm.data()), zm.size());
        msgpack::type::ext ext = oh.get().as<msgpack::type::ext>();
        cout<<"ext type: "<<int(ext.type())<<endl;

        msgpack::object_handle oh2 = msgpack::unpack(ext.data(), ext.size());
        msgpack::object obj = oh2.get();
        obj.convert(message);
        // cout << ">> e3: " << e3 << endl;
        message.accel.print();
        // printf("e final: %d\n",e.type);
    }
    catch (const std::exception &e){
        std::cout << "*** " << e.what() << " ***" << std::endl;
    }
    return message;
}



int main(){
    // printf("hello\n");
    //
    // vec_t a(1,-2,3.3), b(1,-2,3.3), c(3,3,3);
    // assert(a == b);
    //
    // MsgPack<vec_t> buffer;
    // zmq::message_t msg = buffer.pack(a);
    // c = buffer.unpack(msg);
    // assert(a == c);

    vec_t v(1000,-1,0.0001);
    imu_t a(v,v,v), b(v,v,v), c;

    MsgPack<imu_t> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    assert(a == c);
    assert(a.timestamp == c.timestamp);

    a.print();
    c.print();

    // string s = "hello";
    // MsgPack<std::string> b2;
    // zmq::message_t m2 = b2.pack(s);
    // cout << buffer.unpack(m2) << endl;

    return 0;
}

// int main() {
//     std::string val = "ABC";
//     msgpack::type::ext e1(42, val.data(), val.size());
//
//     assert(e1.type() == 42);
//     assert(e1.size() == 3);
//     assert(e1.data()[0] == 'A');
//     assert(e1.data()[1] == 'B');
//     assert(e1.data()[2] == 'C');
//
//     std::stringstream ss;
//     msgpack::pack(ss, e1);
//
//     auto oh = msgpack::unpack(ss.str().data(), ss.str().size());
//     auto e2 = oh.get().as<msgpack::type::ext>();
//     assert(e1 == e2);
//     cout << ">> e2: " << int(e2.type()) << " " << e2.size() << " " << e2.data() << std::endl;
// }
