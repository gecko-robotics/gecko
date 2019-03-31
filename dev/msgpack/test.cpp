#include <stdio.h>
#include <stdint.h>
#include <msgpack.hpp>
#include <string>
#include <vector>
#include <array>
#include <assert.h>
#include <sstream>
#include <iostream>
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "gecko.hpp"

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

class pt_t {
public:
    pt_t(){}
    pt_t(const pt_t& p){angle=p.angle;range=p.range;}
    pt_t(double a, double r){angle=a;range=r;}
    double angle;
    double range;

    MSGPACK_DEFINE(angle, range);
};

class lidar_t  {
public:
    lidar_t(): type(20), timestamp(123.456) {}
    // lidar_t(const imu_t& i): type(10), timestamp(i.timestamp), accel(i.accel), gyro(i.gyro), mag(i.mag) {}
    // lidar_t(const vec_t& a, const vec_t& g, const vec_t& m): type(10), timestamp(123.456), accel(a), gyro(g), mag(m) {}
    vector<pt_t> data;
    double timestamp;
    int type;

    void print() const {
        printf("LIDAR [%f]\n", timestamp);
        for(const auto p: data) printf(" (%.2f, %.2f)",p.angle, p.range);
        printf("\n");
    }
    //
    // bool operator==(const imu_t& v) const {
    //     if((accel == v.accel) && (gyro == v.gyro) && (mag == v.mag) && (type == v.type)) return true;
    //     return false;
    // }

    MSGPACK_DEFINE(data, timestamp);
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




// //-------------------
//
// msgpack::type::ext packVector(const vec_t& v){
//     std::stringstream ss;
//     msgpack::pack(ss, v);
//     msgpack::type::ext e(v.type, ss.str().data(), ss.str().size());
//     return e;
// }
//
// // -----------------------

template<class msg>
zmq::message_t MsgPack<msg>::pack(const msg& m){

    // if (m.type == 10){
    //     msgpack::type::ext a = packVector(m.accel);
    //     msgpack::type::ext g = packVector(m.gyro);
    //     msgpack::type::ext ma = packVector(m.mag);
    //
    //     std::stringstream ss;
    //     msgpack::pack(ss,a);
    //     msgpack::pack(ss,g);
    //     msgpack::pack(ss,ma);
    //
    //     msgpack::type::ext e(m.type, ss.str().data(), ss.str().size());
    //
    //     std::stringstream sss;
    //     msgpack::pack(sss, e);
    //
    //     zmq::message_t z(static_cast<void*>(sss.str().data()), sss.str().size());
    //     return z;
    // }
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

    msg m;
    try {
        // msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size());
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(zm.data()), zm.size());
        msgpack::type::ext ext = oh.get().as<msgpack::type::ext>();
        cout<<" ext type: "<<int(ext.type())<<endl;
        // cout << " > data: " << "  size: " << int(ext.size()) << "  d:" << double(ext.data()[0]) << endl;

        msgpack::object_handle oh2 = msgpack::unpack(ext.data(), ext.size());
        // cout << oh2 << endl;

        msgpack::object obj = oh2.get();
        cout << obj << endl;

        // msg m;
        obj.convert(m);
        m.print();
    }
    catch (const std::exception &e){
        std::cout << "*** " << e.what() << " ***" << std::endl;
    }
    return m;
}


// void sub()
// {
//     reqrep_t t = {"hi", "hello",12345};
//     reqrep_t t2 = {"hi2"};
//     cout << t.topic << t.endpoint << endl;
//     cout << t2.topic << t2.endpoint << endl;
//     // string endpt("tcp://127.0.0.1:12900");
//     string endpt("ipc:///tmp/0");
//     Subscriber s(endpt);
//     MsgPack<imu_t> buffer;
//
//     double last = 0;
//     while (true)
//     {
//         zmq::message_t msg = s.recv();
//         imu_t m = buffer.unpack(msg);
//         cout << "time diff: " << m.timestamp - last << endl;
//         last = m.timestamp;
//     }
// }

void sub()
{
    // string endpt("tcp://127.0.0.1:12900");
    // string endpt("ipc:///tmp/0");
    string endpt = zmqUDS("/tmp/0");
    Subscriber s(endpt);

    MsgPack<imu_t> buffer;

    while (gecko::ok())
    {
        zmq::message_t msg = s.recv();
        imu_t m = buffer.unpack(msg);
        m.print();
        // cout << "time diff: " << m.timestamp - last << endl;
        // last = m.timestamp;
    }
}

void pub()
{
    // string endpt("tcp://127.0.0.1:12900");
    // string endpt("ipc:///tmp/0");
    string endpt = zmqUDS("/tmp/0");
    Publisher p(endpt);
    Rate rate(1);

    MsgPack<imu_t> buffer;

    while (gecko::ok())
    {
        vec_t a(1,2,3);
        imu_t b(a,a,a);  // new timestamp
        zmq::message_t msg = buffer.pack(b);

        p.pub(msg);

        std::cout << "pub" << std::endl;
        // sleep(1);
        // usleep(100000);
        rate.sleep();
    }
}

void pub_lidar()
{
    // string endpt("tcp://127.0.0.1:12900");
    // string endpt("ipc:///tmp/0");
    string endpt = zmqUDS("/tmp/0");
    Publisher p(endpt);
    Rate rate(1);

    MsgPack<lidar_t> buffer;

    while (gecko::ok())
    {
        lidar_t l;
        l.data.push_back(pt_t(1.234, 45.67));
        l.data.push_back(pt_t(1.234, 45.67));
        l.data.push_back(pt_t(1.234, 45.67));
        zmq::message_t msg = buffer.pack(l);

        std::cout << "pub: " << msg << std::endl;

        p.pub(msg);
        // sleep(1);
        // usleep(100000);
        rate.sleep();
    }
}

void sub_lidar()
{
    // string endpt("tcp://127.0.0.1:12900");
    // string endpt("ipc:///tmp/0");
    string endpt = zmqUDS("/tmp/0");
    Subscriber s(endpt);

    MsgPack<lidar_t> buffer;

    while (gecko::ok())
    {
        zmq::message_t msg = s.recv();
        lidar_t m = buffer.unpack(msg);
        m.print();
        // cout << "time diff: " << m.timestamp - last << endl;
        // last = m.timestamp;
    }
}

int main(){
    // pub();
    // sub();

    // pub_lidar();
    sub_lidar();

    // printf("hello\n");
    //
    // vec_t a(1,-2,3.3), b(1,-2,3.3), c(3,3,3);
    // assert(a == b);
    //
    // MsgPack<vec_t> buffer;
    // zmq::message_t msg = buffer.pack(a);
    // c = buffer.unpack(msg);
    // assert(a == c);

    // vec_t v(1000,-1,0.0001);
    // imu_t a(v,v,v), b(v,v,v), c;
    //
    // MsgPack<imu_t> buffer;
    // zmq::message_t msg = buffer.pack(a);
    // c = buffer.unpack(msg);
    // assert(a == c);
    // assert(a.timestamp == c.timestamp);
    //
    // a.print();
    // c.print();

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
