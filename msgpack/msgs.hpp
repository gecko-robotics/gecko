#pragma once

#include <string>
// #include <ctime>
#include <stdio.h>
#include <stdint.h>
#include <msgpack.hpp>

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
 * FIXME: using msgpack, I question the value of this
 */
class base_t {
public:
    base_t(uint8_t t): type(t) {}
    uint8_t type;
    MSGPACK_DEFINE(type);
};


#include "common.hpp"
#include "stamped.hpp"
#include "serialization.hpp"
