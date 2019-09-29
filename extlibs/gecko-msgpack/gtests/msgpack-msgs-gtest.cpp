#include <gtest/gtest.h> // googletest header file
#include "gecko/gecko.hpp"
#include "gecko/msgpack/msgs.hpp"
#include <string>
#include <tuple>
#include <vector>
#include <any>
#include <array>

using namespace std;

// ASSERT_THROW(statement,exception_type)
// ASSERT_ANY_THROW(statement)
// ASSERT_NO_THROW(statement)
// ASSERT_TRUE(statement)


// TEST(msgpack, all) {
//     // tuple msgs = {vec_t}
//     vector<any> msgs = {
//         vec_t(1,2,3)
//     };
//
//     for (auto& m: msgs){
//         zmq::message_t msg = m.pack();
//         auto a(msg);
//         ASSERT_EQ(m, a);
//     }
// }

template<class T>
void msg(T a){
    ASSERT_NO_THROW(a.pack());
    zmq::message_t msg = a.pack();
    ASSERT_NO_THROW(T dummy(msg));
    T c(msg);
    ASSERT_EQ(a,c);
    ASSERT_TRUE(a == c);
}

template<class T>
void msg_st(T a){
    ASSERT_NO_THROW(a.pack());
    zmq::message_t msg = a.pack();
    ASSERT_NO_THROW(T dummy(msg));
    T c(msg);
    ASSERT_EQ(a,c);
    ASSERT_EQ(a.timestamp, c.timestamp);
    ASSERT_TRUE(a == c);
}

TEST(msgpack, vec_t) {
    vec_t a(1,-2,0.33);
    msg<vec_t>(a);
}

TEST(msgpack, quaternion_t) {
    quaternion_t a(1,2,3.3,4);
    msg<quaternion_t>(a);
}

TEST(msgpack, twist_t) {
    vec_t v(1,1,1);
    twist_t a(v,v);
    msg<twist_t>(a);
}

TEST(msgpack, pose_t) {
    vec_t v(1,1,1);
    quaternion_t q(1,0,0,0);
    pose_t a(v,q);
    msg<pose_t>(a);
}

TEST(msgpack, wrench_t) {
    vec_t v(1,1,1);
    wrench_t a(v,v);
    msg<wrench_t>(a);
}

TEST(msgpack, camera_t) {
    array<double,5> D = {1,2,3,4,5};
    array<double,9> K = {1,2,3,4,5,6,7,8,9};
    array<double,9> R = {1,2,3,4,5,6,7,8,9};
    array<double,12> P = {1,2,3,4,5,6,7,8,9,10,11,12};
    camera_t a(200,200,D,K,R,P);
    msg<camera_t>(a);
}

TEST(msgpack, imu_st) {
    vec_t v(0.000001,1000,-1);
    imu_st a(v,v,v);
    msg_st<imu_st>(a);
}

TEST(msgpack, lidar_st) {
    lidar_st a;
    for (int i=0; i<5; i++) a.data.push_back(pt_t(i*50, i*2.1));
    msg_st<lidar_st>(a);
}

// // TEST(msgpack, twist_st) {
// //     vec_t v(1, 1, 1);
// //     twist_st a(v, v), b(v, v), c;
// //     ASSERT_EQ(a,b);
// //     ASSERT_FALSE(a == c);
// //
// //     MsgPack<twist_st> buffer;
// //     zmq::message_t msg = buffer.pack(a);
// //     c = buffer.unpack(msg);
// //     ASSERT_EQ(a, c);
// //     ASSERT_EQ(a.timestamp, c.timestamp);
// // }
// //
// // TEST(msgpack, pose_st) {
// //     vec_t v(1, 1, 1);
// //     quaternion_t q(1, 0, 0, 0);
// //     pose_st a(v,q), b(v,q), c;
// //     ASSERT_EQ(a,b);
// //     ASSERT_FALSE(a == c);
// //
// //     MsgPack<pose_st> buffer;
// //     zmq::message_t msg = buffer.pack(a);
// //     c = buffer.unpack(msg);
// //     ASSERT_EQ(a, c);
// //     ASSERT_EQ(a.timestamp, c.timestamp);
// // }
