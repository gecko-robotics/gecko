/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include "gecko/msgpack/msgs.hpp"
#include <stdio.h>
#include <iostream>

// https://stackoverflow.com/questions/4170745/serializing-opencv-mat-vec3f
// https://stackoverflow.com/questions/29350993/serialize-opencv-mat-using-json-in-c
// try imencode(".jpg", InputArray img, vector<uchar>& buf) -> single row image


// class String: public base_t {
//     std::string
// }

// #define GECKO_MSG(name, id) \
// name(const zmq::message_t& zm): base_t(id) { \
//     msgpack::object obj = unpack(zm); \
//     obj.convert(*this); \
// } \
// zmq::message_t pack(){ \
//     std::stringstream ssmsg; \
//     msgpack::pack(ssmsg, *this); \
//     return base_t::pack(ssmsg); \
// }

class vec_t: public base_t {
public:
    vec_t(): base_t(GVECTOR) {}
    vec_t(const vec_t& a): base_t(GVECTOR) {
        x = a.x;
        y = a.y;
        z = a.z;
        // std::cout << "wtf" << std::endl;
    }
    vec_t(double a, double b, double c): base_t(GVECTOR){
        x = a;
        y = b;
        z = c;
    }
    GECKO_MSG(vec_t, GVECTOR);

    // vec_t(const zmq::message_t& zm): base_t(GVECTOR) {
    //     std::cout << zm << std::endl;
    //     msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(zm.data()), zm.size());
    //     msgpack::type::ext ext = oh.get().as<msgpack::type::ext>();
    //     // std::cout << " ext type: " << int(ext.type()) << std::endl;
    //     // cout << " > data: " << "  size: " << int(ext.size()) << "  d:" << double(ext.data()[0]) << endl;
    //
    //     msgpack::object_handle oh2 = msgpack::unpack(ext.data(), ext.size());
    //     // cout << oh2 << endl;
    //
    //     msgpack::object obj = oh2.get();
    //     // msgpack::object obj = unpack(zm);
    //     // obj.convert(reinterpret_cast<vec_t>(*this));
    //     obj.convert(*this);
    //     // vec_t tmp;
    //     // obj.convert(tmp);
    //     // tmp = obj.as<vec_t>();
    //     // x = tmp.x;
    //     // y = tmp.y;
    //     // z = tmp.z;
    //     // *this = obj.as<vec_t>();
    // }
    //
    // zmq::message_t pack(){
    //     std::stringstream ssmsg;
    //     msgpack::pack(ssmsg, *this);
    //     return base_t::pack(ssmsg);
    // }


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
    friend std::ostream& operator<<(std::ostream& os, const vec_t& v){
        os << "[" << v.x << " " << v.y << " " << v.z << "]";
        return os;
    }
    bool operator==(const vec_t& v) const {
        if((x == v.x) && (y == v.y) && (z == v.z) && (type == v.type)) return true;
        return false;
    }
    void print(char end='\n') const {
        printf(" [%f %f %f]%c", x, y, z,end);
    }

    double x,y,z;
    MSGPACK_DEFINE(x,y,z);
};

class quaternion_t: public base_t {
public:
    quaternion_t(): base_t(GQUATERNION) {}
    quaternion_t(double a, double b, double c, double d): base_t(GQUATERNION), w(a), x(b), y(c), z(d) {}
    quaternion_t(const quaternion_t& q): base_t(GQUATERNION), w(q.w), x(q.x), y(q.y), z(q.z) {}
    // quaternion_t(const zmq::message_t& zm): base_t(GQUATERNION) {
    //     msgpack::object obj = unpack(zm);
    //     obj.convert(*this);
    // }
    // zmq::message_t pack(){
    //     std::stringstream ssmsg;
    //     msgpack::pack(ssmsg, *this);
    //     return base_t::pack(ssmsg);
    // }
    bool operator==(const quaternion_t& v) const {
        if((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w) && (type == v.type)) return true;
        return false;
    }
    double w,x,y,z;
    void print() const {
        printf(" [%f %f %f %f]\n",w,x,y,z);
    }
    friend std::ostream& operator<<(std::ostream& os, const quaternion_t& q){
        os << "[" << q.w << " " << q.x << " " << q.y << " " << q.z << "]";
        return os;
    }
    MSGPACK_DEFINE(w,x,y,z);
    GECKO_MSG(quaternion_t, GQUATERNION);
};

class twist_t: public base_t {
public:
    twist_t(): base_t(GTWIST) {}
    twist_t(vec_t a, vec_t b): base_t(GTWIST),linear(a), angular(b) {}
    // twist_t(const zmq::message_t& zm): base_t(GTWIST) {
    //     msgpack::object obj = unpack(zm);
    //     obj.convert(*this);
    // }
    // zmq::message_t pack(){
    //     std::stringstream ssmsg;
    //     msgpack::pack(ssmsg, *this);
    //     return base_t::pack(ssmsg);
    // }
    vec_t linear, angular;
    bool operator==(const twist_t& v) const {
        if((linear == v.linear) && (angular == v.angular) && (type == v.type)) return true;
        return false;
    }
    MSGPACK_DEFINE(linear, angular);
    GECKO_MSG(twist_t, GTWIST);
};

// [[deprecated]]
// typedef twist_t Twist;

class wrench_t: public base_t {
public:
    wrench_t(): base_t(GWRENCH) {}
    wrench_t(vec_t a, vec_t b): base_t(GWRENCH), force(a), torque(b) {}
    // wrench_t(const zmq::message_t& zm): base_t(GWRENCH) {
    //     msgpack::object obj = unpack(zm);
    //     obj.convert(*this);
    // }
    // zmq::message_t pack(){
    //     std::stringstream ssmsg;
    //     msgpack::pack(ssmsg, *this);
    //     return base_t::pack(ssmsg);
    // }

    vec_t force, torque;

    bool operator==(const wrench_t& v) const {
        if((force == v.force) && (torque == v.torque) && (type == v.type)) return true;
        return false;
    }
    MSGPACK_DEFINE(force, torque);
    GECKO_MSG(wrench_t, GWRENCH);
};


// [[deprecated]]
// typedef wrench_t Wrench;

class pose_t: public base_t {
public:
    pose_t(): base_t(GPOSE) {}
    pose_t(vec_t p, quaternion_t q): base_t(GPOSE), position(p), orientation(q) {}
    pose_t(const pose_t& p): base_t(GPOSE), position(p.position), orientation(p.orientation) {}
    // pose_t(const zmq::message_t& zm): base_t(GPOSE) {
    //     msgpack::object obj = unpack(zm);
    //     obj.convert(*this);
    // }
    // zmq::message_t pack(){
    //     std::stringstream ssmsg;
    //     msgpack::pack(ssmsg, *this);
    //     return base_t::pack(ssmsg);
    // }
    vec_t position;
    quaternion_t orientation;

    bool operator==(const pose_t& v) const {
        if((position == v.position) && (orientation == v.orientation) && (type == v.type)) return true;
        return false;
    }

    void print() const {
        printf("pose_t\n");
        position.print();
        orientation.print();
    }
    MSGPACK_DEFINE(position, orientation);
    GECKO_MSG(pose_t, GPOSE);
};


/****
 * Taken from: http://docs.ros.org/melodic/api/sensor_msgs/html/msg/CameraInfo.html
 *
 * The distortion parameters, size depending on the distortion model.
 * For "plumb_bob", the 5 parameters are: (k1, k2, t1, t2, k3).
 *
 * Intrinsic camera matrix for the raw (distorted) images.
 *     [fx  0 cx]
 * K = [ 0 fy cy]
 *     [ 0  0  1]
 * Projects 3D points in the camera coordinate frame to 2D pixel
 * coordinates using the focal lengths (fx, fy) and principal point
 * (cx, cy).
 *
 * Rectification matrix (stereo cameras only)
 * A rotation matrix aligning the camera coordinate system to the ideal
 * stereo image plane so that epipolar lines in both stereo images are
 * parallel.
 *
 * Projection/camera matrix
 *     [fx'  0  cx' Tx]
 * P = [ 0  fy' cy' Ty]
 *     [ 0   0   1   0]
 * By convention, this matrix specifies the intrinsic (camera) matrix
 *  of the processed (rectified) image. That is, the left 3x3 portion
 *  is the normal camera intrinsic matrix for the rectified image.
 * It projects 3D points in the camera coordinate frame to 2D pixel
 *  coordinates using the focal lengths (fx', fy') and principal point
 *  (cx', cy') - these may differ from the values in K.
 * For monocular cameras, Tx = Ty = 0. Normally, monocular cameras will
 *  also have R = the identity and P[1:3,1:3] = K.
 * For a stereo pair, the fourth column [Tx Ty 0]' is related to the
 *  position of the optical center of the second camera in the first
 *  camera's frame. We assume Tz = 0 so both cameras are in the same
 *  stereo image plane. The first camera always has Tx = Ty = 0. For
 *  the right (second) camera of a horizontal stereo pair, Ty = 0 and
 *  Tx = -fx' * B, where B is the baseline between the cameras.
 * Given a 3D point [X Y Z]', the projection (x, y) of the point onto
 *  the rectified image is given by:
 *  [u v w]' = P * [X Y Z 1]'
 *         x = u / w
 *         y = v / w
 *  This holds for both images of a stereo pair.
 */
class camera_t: public base_t {
public:
    camera_t(): base_t(GCAMERA) {}
    camera_t(
        uint32_t height, uint32_t width,
        const std::array<double,5>& D,
        const std::array<double,9>& K,
        const std::array<double,9>& R,
        const std::array<double,12>& P
    ): base_t(GCAMERA),
        D(D), K(K), R(R), P(P),
        height(height), width(width) {}
    camera_t(const camera_t& v):
        base_t(GCAMERA), D(v.D), K(v.K), R(v.R), P(v.P),
        height(v.height), width(v.width) {}

    bool operator==(const camera_t& v) const {
        if(
            (D == v.D) &&
            (K == v.K) &&
            (R == v.R) &&
            (P == v.P) &&
            (height == v.height) &&
            (width == v.width)
        ) return true;
        return false;
    }

    uint32_t height, width;
    std::array<double, 5> D;  // distortion matrix
    std::array<double, 9> K;  // camera matrix
    std::array<double, 9> R;  // rectification matrix
    std::array<double, 12> P; // projection matrix

    void print() const {
        // printf(" [%f %f %f %f]\n",w,x,y,z);
    }
    MSGPACK_DEFINE(height, width, D, K, R, P);
    GECKO_MSG(camera_t, GCAMERA);
};
