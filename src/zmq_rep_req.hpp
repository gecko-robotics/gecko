#pragma once

#include "transport.hpp"
#include <stdint.h>
#include <string>
#include "zmq.hpp"

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
    inline zmq::message_t get_nb(zmq::message_t& req){return get(req, ZMQ_DONTWAIT);}
protected:
};
