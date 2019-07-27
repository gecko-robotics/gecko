/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

// #include <string>
//
// namespace gecko {
//
// static std::string mc_addr = {"224.3.29.110"};
// static int mc_port = 11311;
//
// }

inline constexpr int  G_EXIT_FAILURE = 1;
inline constexpr int  G_EXIT_SUCCESS = 0;

#define GECKO_DEPRECATED(msg) [[deprecated(msg)]]

// Just add gecko.hpp and all of these get added
#include "gecko/geckocpp.hpp"    // geckocpp stuff
#include "gecko/time.hpp"        // rate
#include "gecko/node.hpp"        // threads
#include "gecko/transport.hpp"   // base zmq
#include "gecko/zmq_sub_pub.hpp"  // pub/sub
#include "gecko/zmq_rep_req.hpp"  // req/rep ... need to finish
#include "gecko/helpers.hpp"     // tcp://x.x.x.x:port
#include "gecko/network.hpp"     // ip hostname
#include "gecko/exceptions.hpp"  // NotImplemented
#include "gecko/color.hpp"       // ColorPrint ... doesn't work on all OS'es
#include "gecko/log.hpp"         // logging, local and remote
#include "gecko/signals.hpp"  // capture ctrl-c
#include "gecko/msocket.hpp"  // beacon
#include "gecko/ps.hpp"       // process performance
#include "gecko/ascii.hpp"    // ascii message parser
#include "gecko/db.hpp"       // database for core
#include "gecko/core.hpp"     // geckocore/beaconcoreserver
#include "gecko/json.hpp"     // read/write json files

#include <unistd.h>           // sleep, usleep

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"
// #include "zmq_addon.hpp"
