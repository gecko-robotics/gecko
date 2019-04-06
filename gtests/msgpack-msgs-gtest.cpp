#include <gtest/gtest.h> // googletest header file
#include "gecko/gecko.hpp"
#include "msgpack/msgs.hpp"
#include <string>

using namespace std;

// ASSERT_THROW(statement,exception_type)
// ASSERT_ANY_THROW(statement)
// ASSERT_NO_THROW(statement)
// ASSERT_TRUE(statement)

TEST(msgpack, vec_t) {
    vec_t a(1,-2,3.3), b(1,-2,3.3), c(3,3,3);
    ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<vec_t> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
}

TEST(msgpack, quaternion_t) {
    quaternion_t a(1,-2,3.3,4), b(1,-2,3.3,4), c;
    ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<quaternion_t> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
}

TEST(msgpack, twist_t) {
    vec_t v(1,1,1);
    twist_t a(v,v), b(v,v), c;
    ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<twist_t> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
}

TEST(msgpack, pose_t) {
    vec_t v(1,1,1);
    quaternion_t q(1,0,0,0);
    pose_t a(v,q), b(v,q), c;
    ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<pose_t> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
}

TEST(msgpack, wrench_t) {
    vec_t v(1,1,1);
    wrench_t a(v,v), b(v,v), c;
    ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<wrench_t> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
}

TEST(msgpack, twist_st) {
    vec_t v(1,1,1);
    twist_st a(v,v), b(v,v), c;
    ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<twist_st> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
    ASSERT_EQ(a.timestamp,c.timestamp);
}

TEST(msgpack, pose_st) {
    vec_t v(1,1,1);
    quaternion_t q(1,0,0,0);
    pose_st a(v,q), b(v,q), c;
    ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<pose_st> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
    ASSERT_EQ(a.timestamp,c.timestamp);
}

TEST(msgpack, imu_st) {
    vec_t v(0.000001,1000,-1);
    imu_st a(v,v,v), b(v,v,v), c;
    // ASSERT_EQ(a,b);
    ASSERT_FALSE(a == c);

    MsgPack<imu_st> buffer;
    zmq::message_t msg = buffer.pack(a);
    c = buffer.unpack(msg);
    ASSERT_EQ(a,c);
    ASSERT_EQ(a.timestamp,c.timestamp);
}
