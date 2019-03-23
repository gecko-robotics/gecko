#pragma once

#include "msgs.hpp"
#include "common.hpp"
#include <ctime>
#include <sys/time.h>

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
    imu_t(const vec_t& a, const vec_t& g, const vec_t& m): base_t(GIMU), accel(a), gyro(g), mag(m) {}
    vec_t accel, gyro, mag;

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

    // MSGPACK_DEFINE(accel, gyro, mag, MSGPACK_BASE(msg_t));
    MSGPACK_DEFINE(accel, gyro, mag, timestamp);
};

class twist_st: public twist_t, public msg_t {
public:
    twist_st() {}
    twist_st(const vec_t& l, const vec_t& a): twist_t(l,a) {}
    // bool operator==(const twist_st& v) const {
    //     if((linear == v.linear) && (angular == v.angular) && (type == v.type)) return true;
    //     return false;
    // }
    // vec_t linear, angular;
    MSGPACK_DEFINE(linear, angular, timestamp);
    // MSGPACK_DEFINE(MSGPACK_BASE(twist_t), MSGPACK_BASE(msg_t));
};

class pose_st: public pose_t, public msg_t {
public:
    pose_st() {}
    pose_st(vec_t p, quaternion_t q): Pose(p, q) {}
    pose_st(const pose_st& p): Pose(p.position, p.orientation), msg_t(p.timestamp) {}

    void print() const {printf("pose_st [%f]\n", timestamp);}
    MSGPACK_DEFINE(MSGPACK_BASE(pose_t), timestamp);
};

[[deprecated]]
typedef  pose_st PoseStamped;

class LidarPt {
public:
    LidarPt(){}
    LidarPt(double a, double r){
        angle = a;
        range = r;
    }
    double angle, range;
    MSGPACK_DEFINE(angle, range);
};

class lidar_st: public base_t, public msg_t {
public:
    lidar_st(): base_t(GLIDAR) {}
    std::vector<LidarPt> scan;
    MSGPACK_DEFINE(scan, timestamp);
};

[[deprecated]]
typedef  lidar_st Lidar;

class image_st: public base_t, public msg_t {
public:
    image_st(): base_t(GIMAGE) {}
    // std::vector<LidarPt> scan;
    int width, height, depth;
    MSGPACK_DEFINE(width, height, depth, timestamp);
};

[[deprecated]]
typedef  image_st Image;
