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
    imu_t(vec_t a, vec_t g, vec_t m): base_t(GIMU), accel(a), gyro(g), mag(m) {}
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

    MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), accel, gyro, mag);
};

class TwistStamped: public base_t, public msg_t {
public:
    TwistStamped(): base_t(GTWIST) {}
    vec_t linear, angular;
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), linear, angular);
};

class PoseStamped: public Pose, public msg_t {
public:
    PoseStamped() {}
    PoseStamped(vec_t p, quaternion_t q): Pose(p, q) {}
    PoseStamped(const PoseStamped& p): Pose(p.position, p.orientation), msg_t(p.timestamp) {}

    void print() const {printf("PoseStamped [%f]\n", timestamp);}
    MSGPACK_DEFINE(MSGPACK_BASE(Pose), MSGPACK_BASE(msg_t));
};

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

class Lidar: public base_t, public msg_t {
public:
    Lidar(): base_t(GLIDAR) {}
    std::vector<LidarPt> scan;
    MSGPACK_DEFINE(MSGPACK_BASE(base_t), MSGPACK_BASE(msg_t), scan);
};
