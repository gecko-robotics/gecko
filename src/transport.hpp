#pragma once

#include <string>
#include <array>
#include <iostream>
#include "zmq.hpp"
#include "time.hpp"

// Returns the type of zmq connection as a colored string
// Useful for debugging
class zmqType {
public:
    zmqType(int i): type(i) {}
    std::string to_string();
    const char* c_str();
    int type;
};

class zmqBase {
public:
    zmqBase(int type);
    ~zmqBase();

    // void setEndPt();  // determine ip:port that was given by OS
    void close();  // close socket
    bool check(int retry=5);  // select: has data arrived to read?
    static zmq::context_t gContext;  // zmq context
    zmq::socket_t sock;
    std::string endpoint;  // tcp://x.x.x.x:port
    bool bind;  // was socket bound or connected?
    int type;  // pub/sub/etc
protected:
    void setEndPt();  // determine ip:port that was given by OS
};

class Publisher: public zmqBase {
public:
    Publisher();
    Publisher(std::string addr, bool bind=true);  // tcp://x.x.x.x:port
    void pub(zmq::message_t& msg);

protected:

};


class Subscriber: public zmqBase {
public:
    Subscriber();
    Subscriber(std::string addr, std::string topic, bool bind=false);
    Subscriber(std::string addr, bool bind=false);
    zmq::message_t recv(int flags=0);
    inline zmq::message_t recv_nb(){return recv(ZMQ_DONTWAIT);}
    void setCallback(void(*callback)(zmq::message_t&));
// protected:
    void(*callback)(zmq::message_t&);
};

class Reply: public zmqBase {
public:
    Reply(std::string addr);
    void listen(zmq::message_t(*)(zmq::message_t&), int flags=0);
    inline void listen_nb(zmq::message_t(*cb)(zmq::message_t&)){listen(cb,ZMQ_DONTWAIT);}
protected:
};


class Request: public zmqBase {
public:
    Request(std::string);
    zmq::message_t get(zmq::message_t&,int flags=0);
    zmq::message_t get_nb(zmq::message_t& req){return get(req, ZMQ_DONTWAIT);}
protected:
};
