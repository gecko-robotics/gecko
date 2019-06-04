/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

// These are the headers a user would access. Others are for internal
// use only.
#include "gecko/geckocpp.hpp"
#include "gecko/time.hpp"        // rate
#include "gecko/node.hpp"        // threads
#include "gecko/transport.hpp"   // pub-sub

#include "gecko/zmq_sub_pub.hpp"
#include "gecko/zmq_rep_req.hpp"

#include "gecko/helpers.hpp"     // tcp://x.x.x.x:port
#include "gecko/network.hpp"     // ip hostname
#include "gecko/exceptions.hpp"  // NotImplemented
#include "gecko/color.hpp"       // ColorPrint
#include "gecko/log.hpp"         // logging, local and remote
#include "gecko/signals.hpp"
#include "gecko/msocket.hpp"
#include "gecko/ascii.hpp"
#include "gecko/core.hpp"
#include <unistd.h>        // sleep, usleep

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"
// #include "zmq_addon.hpp"
