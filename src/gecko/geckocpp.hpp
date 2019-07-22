/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <string>
#include "transport.hpp"  // pub/sub
#include "zmq_sub_pub.hpp"
// #include "zmq_rep_req.hpp"


namespace gecko {

    // functions ---------------------------------
    void init(std::string mc={""}, int port=0);  // FIXME
    bool ok();  // still running? true/false
    inline bool is_shutdown(){return !ok();} // stopping? true/false
    void shutdown(); // shutdown gecko

    Subscriber* subBindTCP(std::string key, std::string topic);
    Subscriber* subBindUDS(std::string key, std::string topic, std::string file);
    Subscriber* subConnectTCP(std::string key, std::string topic);
    Subscriber* subConnectUDS(std::string key, std::string topic);

    Publisher* pubBindTCP(std::string key, std::string topic);
    Publisher* pubBindUDS(std::string key, std::string topic, std::string file);
    Publisher* pubConnectTCP(std::string key, std::string topic);
    Publisher* pubConnectUDS(std::string key, std::string topic);

    enum LogLevel {INFO, WARN, ERROR, DEBUG};

    void log(int level, const std::string& s);
};
