/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

// #include <string>
// #include <stdio.h>
#include <stdint.h>
#include <msgpack.hpp>

enum GeckoMsgs {
    GVECTOR    = 0,
    GQUATERNION= 1,
    GWRENCH    = 2,
    GPOSE      = 3,
    GJOYSTICK  = 4,
    GTWIST     = 5,
    GIMU       = 10,
    GLIDAR     = 11,
    GIMAGE     = 12
};

/**
 * Base class for keeping track of message type
 */
class base_t {
public:
    base_t(uint8_t t): type(t) {}
    uint8_t type;
};


#include "common.hpp"
#include "stamped.hpp"
#include "serialization.hpp"
