/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <string>
#include <gecko/transport.hpp>  // pub/sub
#include <gecko/zmq_sub_pub.hpp>
// #include "zmq_rep_req.hpp"


namespace gecko {

    // functions ---------------------------------
    // void init(std::string mc={""}, int port=0);  // FIXME
    // void set_port(int port);
    // void set_multicast(std::string mc, int port);
    void set_broadcast(int port);
    void init();
    bool ok();  // still running? true/false
    inline bool is_shutdown(){return !ok();} // stopping? true/false
    void shutdown(); // shutdown gecko

    // void hello();
    int get_broadcast();
    int* getAddress();

    // Creating connections between processes
    //   Bind: can be TCP or UDS for either pub/sub
    //   Connect: can be TCP or UDS for either pub/sub
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
