/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <stdint.h>
#include <msgpack.hpp>
#include <string>
#include <array>
#include <sstream>
#include <zmq.hpp>
#include <stdint.h>


/****
 * These are needed for python and the way it packs/unpacks messages.
 */
enum GeckoMsgs {
    GVECTOR    = 0,  // vector: x,y,z
    GQUATERNION= 1,  // quaterion: w,x,y,z
    GWRENCH    = 2,  // wrench: force, torque
    GPOSE      = 3,  // pose: position, orientation
    GJOYSTICK  = 4,  // joystick: axes, buttons
    GTWIST     = 5,  // twist: linear, angular velocity
    GCAMERA    = 6,  // camera info: various calibration matricies
    GIMU       = 10, // imu
    GLIDAR     = 20, // lidar
    GIMAGE     = 12, // image
    GOCC       = 13, // occupancy grid
    GODOM      = 14  // odometry
};

/****
 * This macro helps with creating new messages and not having to write a bunch
 * of boilerplate. It allows pack/unpack of a message.
 */
#define GECKO_MSG(name, id) \
name(const zmq::message_t& zm): base_t(id) { \
    msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(zm.data()), zm.size()); \
    msgpack::type::ext ext = oh.get().as<msgpack::type::ext>();\
    msgpack::object_handle oh2 = msgpack::unpack(ext.data(), ext.size()); \
    msgpack::object obj = oh2.get(); \
    obj.convert(*this); \
} \
zmq::message_t pack(){ \
    std::stringstream ssmsg; \
    msgpack::pack(ssmsg, *this); \
    return base_t::pack(ssmsg); \
}

/****
 * Base class for keeping track of message type
 */
class base_t {
public:
    base_t(uint8_t t): type(t) {}

    zmq::message_t pack(std::stringstream& ssmsg){
        // std::stringstream ssmsg;
        // msgpack::pack(ssmsg, *this);

        // std::cout << "packed msg: " << sss.str().size() << " " << sss.str() << std::endl;

        msgpack::type::ext e1(type, ssmsg.str().data(), ssmsg.str().size());

        // printf(">> packed: %d\n", m.type);

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

    // DOESN'T WORK!!!
    // msgpack::object unpack(const zmq::message_t& zm) {
    //     // try {
    //         // msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size());
    //         msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(zm.data()), zm.size());
    //         msgpack::type::ext ext = oh.get().as<msgpack::type::ext>();
    //         // std::cout << " ext type: " << int(ext.type()) << std::endl;
    //         // cout << " > data: " << "  size: " << int(ext.size()) << "  d:" << double(ext.data()[0]) << endl;
    //
    //         msgpack::object_handle oh2 = msgpack::unpack(ext.data(), ext.size());
    //         // cout << oh2 << endl;
    //
    //         msgpack::object obj = oh2.get();
    //         return obj;
    //         // std::cout << obj << std::endl;
    //
    //         // msg m;
    //         // obj.convert(*this);
    //         // m.print();
    //     // }
    //     // catch (const std::exception &e){
    //     //     std::cout << "*** " << e.what() << " ***" << std::endl;
    //     // }
    // }

    uint8_t type;  // this is use when packing/unpacking primarily in python
};


#include "gecko/msgpack/common.hpp"
#include "gecko/msgpack/stamped.hpp"
#include "gecko/msgpack/serialization.hpp"
