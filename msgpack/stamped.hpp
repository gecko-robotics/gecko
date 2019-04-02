/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include "msgs.hpp"
#include "common.hpp"
#include <ctime>
#include <vector>
#include <sys/time.h>

class msg_st {
public:
    msg_st(const double t): timestamp(t) {}
    msg_st(): timestamp(time_now()) {}
    double time_now(){
        struct timeval ts;
        gettimeofday(&ts, NULL);
        return ((double)(ts.tv_sec) + 1e-6*(ts.tv_usec));
    }
    double timestamp;
    MSGPACK_DEFINE(timestamp);
};

class imu_st: public base_t, public msg_st  {
public:
    imu_st(): base_t(GIMU) {}
    imu_st(const imu_st& i): base_t(GIMU), msg_st(i.timestamp), accel(i.accel), gyro(i.gyro), mag(i.mag) {}
    imu_st(const vec_t& a, const vec_t& g, const vec_t& m): base_t(GIMU), accel(a), gyro(g), mag(m) {}
    vec_t accel, gyro, mag;

    void print() const {
        printf("IMU [%f]\n", timestamp);
        accel.print();
        gyro.print();
        mag.print();
    }

    bool operator==(const imu_st& v) const {
        if((accel == v.accel) && (gyro == v.gyro) && (mag == v.mag) && (type == v.type) && (timestamp == v.timestamp)) return true;
        // if((accel == v.accel) && (gyro == v.gyro) && (mag == v.mag) && (type == v.type)) return true;
        return false;
    }

    // MSGPACK_DEFINE(accel, gyro, mag, MSGPACK_BASE(msg_st));
    MSGPACK_DEFINE(accel, gyro, mag, timestamp);
};

class twist_st: public twist_t, public msg_st {
public:
    twist_st() {}
    twist_st(const vec_t& l, const vec_t& a): twist_t(l,a) {}
    // bool operator==(const twist_st& v) const {
    //     if((linear == v.linear) && (angular == v.angular) && (type == v.type)) return true;
    //     return false;
    // }
    // vec_t linear, angular;
    MSGPACK_DEFINE(linear, angular, timestamp);
    // MSGPACK_DEFINE(MSGPACK_BASE(twist_t), MSGPACK_BASE(msg_st));
};

class pose_st: public pose_t, public msg_st {
public:
    pose_st() {}
    pose_st(const vec_t& p, const quaternion_t& q): pose_t(p, q) {}
    pose_st(const pose_st& p): pose_t(p.position, p.orientation), msg_st(p.timestamp) {}

    void print() const {printf("pose_st [%f]\n", timestamp);}
    MSGPACK_DEFINE(MSGPACK_BASE(pose_t), timestamp);
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

class lidar_st: public base_t, public msg_st  {
public:
    lidar_st(): base_t(20) {}
    // lidar_t(const imu_t& i): type(10), timestamp(i.timestamp), accel(i.accel), gyro(i.gyro), mag(i.mag) {}
    // lidar_t(const vec_t& a, const vec_t& g, const vec_t& m): type(10), timestamp(123.456), accel(a), gyro(g), mag(m) {}
    std::vector<pt_t> data;
    // double timestamp;
    // int type;

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


// [[deprecated]]
// typedef  pose_st PoseStamped;

// class LidarPt {
// public:
//     LidarPt(){}
//     LidarPt(double a, double r){
//         angle = a;
//         range = r;
//     }
//     double angle, range;
//     MSGPACK_DEFINE(angle, range);
// };

// class lidar_st: public base_t, public msg_st {
// public:
//     lidar_st(): base_t(GLIDAR) {}
//     std::vector<LidarPt> scan;
//     MSGPACK_DEFINE(scan, timestamp);
// };

// [[deprecated]]
// typedef  lidar_st Lidar;

// class image_st: public base_t, public msg_st {
// public:
//     image_st(): base_t(GIMAGE) {}
//     // std::vector<LidarPt> scan;
//     int width, height, depth;
//     MSGPACK_DEFINE(width, height, depth, timestamp);
// };
//
// [[deprecated]]
// typedef  image_st Image;
