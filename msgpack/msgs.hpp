#pragma once

#include <string>
#include <ctime>
#include <stdio.h>
#include <stdint.h>

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

// class String: public base_t {
//     std::string
// }

class vec_t: public base_t {
public:
    vec_t(): base_t(GVECTOR) {}
    vec_t(const vec_t& a): base_t(GVECTOR) {
        x = a.x;
        y = a.y;
        z = a.z;
    }
    vec_t(double a, double b, double c): base_t(GVECTOR){
        x = a;
        y = b;
        z = c;
    }
    // double operator[](uint8_t i){
    //     if(i == 0) return x;
    //     else if (i == 1) return y;
    //     else if (i == 2) return z;
    //     else printf("invalide: %d", i);
    //     return 0;
    // }
    // double& operator[](int i){
    //     if(i == 0) return x;
    //     else if (i == 1) return y;
    //     else if (i == 2) return z;
    //     else printf("invalide: %d", i);
    //     return x;
    // }
    // vec_t& operator=(vec_t&& a) noexcept {
    //     x=a.x;
    //     y=a.y;
    //     z=a.z;
    //     return *this;
    // }
    // const uint8_t type;
    void print(char end='\n') const {
        printf(" [%f %f %f]%c", x, y, z,end);
    }

    double x,y,z;
    MSGPACK_DEFINE(MSGPACK_BASE(base_t),x,y,z);
};

class quaternion_t: public base_t {
public:
    quaternion_t(): base_t(GQUATERNION) {}
    quaternion_t(double a, double b, double c, double d): base_t(GQUATERNION), w(a), x(b), y(c), z(d) {}
    quaternion_t(const quaternion_t& q): base_t(GQUATERNION), w(q.w), x(q.x), y(q.y), z(q.z) {}

    double w,x,y,z;
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), w,x,y,z);
};

class msg_t {
public:
    msg_t(const double t): timestamp(t) {}
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
    imu_t(const imu_t& i): base_t(GIMU), msg_t(i.timestamp), accel(i.accel), gyro(i.gyro), mag(i.mag) {}
    imu_t(vec_t a, vec_t g, vec_t m): base_t(GIMU), accel(a), gyro(g), mag(m) {}
    vec_t accel, gyro, mag;

    void print() const {
        printf("IMU [%f]\n", timestamp);
        accel.print();
        gyro.print();
        mag.print();
    }

    MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), accel, gyro, mag);
};

class Twist: public base_t, public msg_t {
public:
    Twist(): base_t(GTWIST) {}
    vec_t linear, angular;
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), linear, angular);
};

class Pose: public base_t, public msg_t {
public:
    Pose(): base_t(GPOSE) {}
    Pose(vec_t p, quaternion_t q): base_t(GPOSE), position(p), orientation(q) {}
    Pose(const Pose& p): base_t(GPOSE), msg_t(p.timestamp), position(p.position), orientation(p.orientation) {}
    vec_t position;
    quaternion_t orientation;

    void print() const {printf("Pose [%f]\n", timestamp);}
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), position, orientation);
};

// class LidarPt {
// public:
//     LidarPt(double a, double r){
//         angle = a;
//         range = r;
//     }
//     double angle, range;
//     MSGPACK_DEFINE(angle, range);
// };
//
// class Lidar: public base_t, public msg_t {
// public:
//     Lidar(): base_t(GLIDAR) {}
//     std::vector<LidarPt> scan;
//     MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), scan);
// };
