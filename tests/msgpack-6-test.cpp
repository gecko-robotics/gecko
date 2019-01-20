/*
This tests:
- all msgpack message formats
- packing messages
- unpacking messages
- ONLY uncompressed transport

*/

#include <msgpack.hpp>
#include <stdio.h>
#include <assert.h>
#include <ctime>
#include <sys/time.h>
#include <vector>
#include <sstream>
#include <iostream>

#include "msgs.hpp"
// #include "stamped.hpp"

using namespace std;


template<class msg>
void get(const std::stringstream& ss, std::vector<msg>& recv){
    size_t offset = 0;
    int loop = 0;
    try {
        while((ss.str().size() - offset) > 0){
            cout << "buffer: " << ss.str().size() << "  offset: " << offset << endl;
            msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
            msgpack::object obj = oh.get();

            // cout << "\nLoop: " << ++loop << " -------------------" << endl;
            // cout << obj << endl;

            msg e;

            obj.convert(e);
            // e.print();

            recv.push_back(e);
        }
    }
    // catch (msgpack::v3::insufficient_bytes& e)
    catch (const std::exception &e){
        cout << "" << e.what() << endl;
        // cout << "Done ..." << endl;
    }

}

void getString(){
    printf("\nString ===========================\n");
    // create data
    string s("hello world!");
    printf("original message:\n");
    printf("%s\n", s.c_str());

    // serialize
    stringstream ss;
    msgpack::pack(ss, s);
    msgpack::pack(ss, s);
    msgpack::pack(ss, s);

    // deserialize
    size_t offset = 0;
    cout << "sizeof(message): " << sizeof(imu_t) << endl;
    cout << "packed size: " << ss.str().size() << endl;
    int loop = 0;

    vector<string> recv;

    get(ss, recv);

    // cout << "\n Vector -------------------" << endl;
    for (const string& v: recv) {
        printf("%s\n", v.c_str());

        assert(s == v);
    }
}

void getIMU(){
    printf("\nIMU ===========================\n");
    // create data
    vec_t a(1.2,3.4,-5.6789);
    vec_t b(-1.2,-3.4,5.6789);
    vec_t c(1e-6,2e3,3e9);

    imu_t d(a,b,c);
    printf("original message:\n");
    d.print();

    // serialize
    stringstream ss;
    msgpack::pack(ss, d);
    msgpack::pack(ss, d);
    msgpack::pack(ss, d);

    // deserialize
    size_t offset = 0;
    cout << "sizeof(message): " << sizeof(imu_t) << endl;
    cout << "packed size: " << ss.str().size() << endl;
    int loop = 0;

    vector<imu_t> recv;

    get(ss, recv);

    // cout << "\n Vector -------------------" << endl;
    for (const imu_t& v: recv) {
        v.print();

        assert(d == v);
    }
}

void getPose(){
    cout << "\nPose ===========================\n";
    // create data
    vec_t a(1.2345,3.45678,5.6789);
    quaternion_t q(1.0,0,0,0);

    Pose p(a,q);
    printf("original message:\n");
    p.print();

    // serialize
    stringstream ss;
    msgpack::pack(ss, p);
    msgpack::pack(ss, p);

    // deserialize
    size_t offset = 0;
    cout << "sizeof(message): " << sizeof(imu_t) << endl;
    cout << "packed size: " << ss.str().size() << endl;
    int loop = 0;

    vector<Pose> recv;

    get(ss, recv);

    // cout << "\nVector -------------------" << endl;
    for (const Pose& v: recv){
        v.print();
        assert(p == v);
        // assert(p.type == v.type);
        // assert(p.position == v.position);
        // assert(p.orientation.x == v.orientation.x);
    }
}

void getPoseStamped(){
    cout << "\nPoseStamped ===========================\n";
    // create data
    vec_t a(1.2345,3.45678,5.6789);
    quaternion_t q(1.0,0,0,0);

    PoseStamped p(a,q);
    printf("original message:\n");
    p.print();

    // serialize
    stringstream ss;
    msgpack::pack(ss, p);
    msgpack::pack(ss, p);

    // deserialize
    size_t offset = 0;
    cout << "sizeof(message): " << sizeof(imu_t) << endl;
    cout << "packed size: " << ss.str().size() << endl;
    int loop = 0;

    vector<PoseStamped> recv;

    get(ss, recv);

    // cout << "\nVector -------------------" << endl;
    for (const PoseStamped& v: recv){
        v.print();
        assert(p.type == v.type);
        assert(p.position.x == v.position.x);
        assert(p.orientation.x == v.orientation.x);
        assert(p.timestamp == v.timestamp);
    }
}

void getLidar(){
    cout << "\nLidar ===========================\n";
    // create data
    LidarPt a(100.0,200.0);
    Lidar l;
    for(int i=0; i < 10; ++i) l.scan.push_back(a);

    printf("original message:\n");
    // p.print();

    // serialize
    stringstream ss;
    msgpack::pack(ss, l);
    msgpack::pack(ss, l);

    // deserialize
    size_t offset = 0;
    cout << "sizeof(message): " << sizeof(imu_t) << endl;
    cout << "packed size: " << ss.str().size() << endl;
    int loop = 0;

    vector<Lidar> recv;

    get(ss, recv);

    for (const Lidar& v: recv){
        assert(v.timestamp == l.timestamp);
        cout << "\nPoints -------------------" << endl;
        for (const LidarPt& p: v.scan) {
            cout << p.angle << " " << p.range << endl;
            assert(p.angle == a.angle);
            assert(p.range == a.range);
            // assert(p.position == v.position);
            // assert(p.orientation.x == v.orientation.x);
        }
    }
}


int main(void) {
    getString();
    getIMU();
    getPose();

    printf("\n===[ Stamped ]=========================================\n");
    getLidar();
    getPoseStamped();

    return 0;
}
