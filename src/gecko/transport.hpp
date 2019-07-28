/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <stdint.h>
#include <string>
#include <zmq.hpp>

// helper functions to create TCP or UDS connections
std::string zmqTCP(const std::string& addr, uint16_t port); // specific port
std::string zmqTCP(const std::string& addr); // get random port number
std::string zmqUDS(const std::string& file); // UDS file, must be on same machine




// Returns the type of zmq connection as a colored string
// Useful for debugging
// FIXME: embed in zmqBase???
// class zmqType {
// public:
//     zmqType(int i): type(i) {}
//     std::string to_string();
//     const char* c_str();
//     int type;
// };

class zmqBase {
public:
    zmqBase(int type);
    ~zmqBase();

    // void setEndPt();  // determine ip:port that was given by OS
    void close();  // close socket
    bool check(uint16_t msec=500);  // select: has data arrived to read?
    void bind(const std::string& addr);
    void connect(const std::string& addr);

    static zmq::context_t gContext;  // zmq context
    // zmq::context_t gContext;  // zmq context
    zmq::socket_t sock;
    std::string endpoint;  // tcp://x.x.x.x:port
    bool bound;  // was socket bound or connected?
    int type;  // pub/sub/etc
protected:
    void setEndPt();  // determine ip:port that was given by OS
    // std::map<int,std::string> = {
    //     {ZMQ_SUB, "Subscriber"},
    //     {ZMQ_PUB, "Publisher"},
    //     {}
    // }
};
