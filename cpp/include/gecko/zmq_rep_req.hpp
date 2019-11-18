/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <gecko/transport.hpp>
#include <stdint.h>
#include <string>
#include <zmq.hpp>

// partially satisfies named requirement BitmaskType
// enum class send_flags : int
// {
//     none = 0,
//     dontwait = ZMQ_DONTWAIT,
//     sndmore = ZMQ_SNDMORE
// };
//
// enum class recv_flags : int
// {
//     none = 0,
//     dontwait = ZMQ_DONTWAIT
// };

class Reply: public zmqBase {
public:
    Reply(std::string addr);
    void listen(zmq::message_t(*)(zmq::message_t&), zmq::recv_flags flags=zmq::recv_flags::none);
    inline void listen_nb(zmq::message_t(*cb)(zmq::message_t&)){listen(cb,zmq::recv_flags::dontwait);}
protected:
};


class Request: public zmqBase {
public:
    Request(std::string);
    zmq::message_t get(zmq::message_t&, zmq::recv_flags flags=zmq::recv_flags::none);
    inline zmq::message_t get_nb(zmq::message_t& req){return get(req, zmq::recv_flags::dontwait);}
protected:
};
