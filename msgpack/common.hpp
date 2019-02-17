#pragma once

#include "msgs.hpp"
#include <stdio.h>


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
    bool operator==(const vec_t& v) const {
        if((x == v.x) && (y == v.y) && (z == v.z) && (type == v.type)) return true;
        return false;
    }
    void print(char end='\n') const {
        printf(" [%f %f %f]%c", x, y, z,end);
    }

    double x,y,z;
    MSGPACK_DEFINE(MSGPACK_BASE(base_t),x,y,z);
};

typedef vec_t point_t;

class quaternion_t: public base_t {
public:
    quaternion_t(): base_t(GQUATERNION) {}
    quaternion_t(double a, double b, double c, double d): base_t(GQUATERNION), w(a), x(b), y(c), z(d) {}
    quaternion_t(const quaternion_t& q): base_t(GQUATERNION), w(q.w), x(q.x), y(q.y), z(q.z) {}
    bool operator==(const quaternion_t& v) const {
        if((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w) && (type == v.type)) return true;
        return false;
    }
    double w,x,y,z;
    void print() const {
        printf(" [%f %f %f %f]\n",w,x,y,z);
    }
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), w,x,y,z);
};

// class msg_t {
// public:
//     msg_t(const double t): timestamp(t) {}
//     msg_t(): timestamp(time_now()) {}
//     double time_now(){
//         struct timeval ts;
//         gettimeofday(&ts, NULL);
//         return ((double)(ts.tv_sec) + 1e-6*(ts.tv_usec));
//     }
//     double timestamp;
//     MSGPACK_DEFINE(timestamp);
// };
//
// class imu_t: public base_t, public msg_t  {
// public:
//     imu_t(): base_t(GIMU) {}
//     imu_t(const imu_t& i): base_t(GIMU), msg_t(i.timestamp), accel(i.accel), gyro(i.gyro), mag(i.mag) {}
//     imu_t(vec_t a, vec_t g, vec_t m): base_t(GIMU), accel(a), gyro(g), mag(m) {}
//     vec_t accel, gyro, mag;
//
//     void print() const {
//         printf("IMU [%f]\n", timestamp);
//         accel.print();
//         gyro.print();
//         mag.print();
//     }
//
//     bool operator==(const imu_t& v) const {
//         if((accel == v.accel) && (gyro == v.gyro) && (mag == v.mag) && (type == v.type)) return true;
//         return false;
//     }
//
//     MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), accel, gyro, mag);
// };
//
class Twist: public base_t {
public:
    Twist(): base_t(GTWIST) {}
    Twist(vec_t a, vec_t b): base_t(GTWIST),linear(a), angular(b) {}
    vec_t linear, angular;
    bool operator==(const Twist& v) const {
        if((linear == v.linear) && (angular == v.angular) && (type == v.type)) return true;
        return false;
    }
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), linear, angular);
};

typedef Twist twist_t;

class Wrench: public base_t {
public:
    Wrench(): base_t(GWRENCH) {}
    Wrench(vec_t a, vec_t b): base_t(GWRENCH), force(a), torque(b) {}

    vec_t force, torque;

    bool operator==(const Wrench& v) const {
        if((force == v.force) && (torque == v.torque) && (type == v.type)) return true;
        return false;
    }
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), force, torque);
};


typedef Wrench wrench_t;

class Pose: public base_t {
public:
    Pose(): base_t(GPOSE) {}
    Pose(vec_t p, quaternion_t q): base_t(GPOSE), position(p), orientation(q) {}
    Pose(const Pose& p): base_t(GPOSE), position(p.position), orientation(p.orientation) {}
    vec_t position;
    quaternion_t orientation;

    bool operator==(const Pose& v) const {
        if((position == v.position) && (orientation == v.orientation) && (type == v.type)) return true;
        return false;
    }

    void print() const {
        printf("Pose\n");
        position.print();
        orientation.print();
    }
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), position, orientation);
};

typedef Pose pose_t;

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
