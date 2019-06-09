/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include "gecko/transport.hpp"
#include <stdint.h>
#include <string>
#include "zmq.hpp"


/* For both sub/pub they follow a similar api:
 *
 *   Publisher p;
 *   p.bind(endpt);  // or p.connect(endpt);
 *   p.publish(msg);
 *
 *   Subscriber s;
 *   s.connect(endpt);  // or s.bind(endpt)
 *   zmq::message_t msg = s.recv();
 *
 * Note that in both cases, the api deals with zmq messages and not a
 * specific serialization library message format. This is mainly because
 * I may switch it later and I don't want a library to deeply embeded in
 * gecko.
 *
 * Also note, zmq allows a pub or sub to either bind or connect depending
 * on how you use it. Only one socket can bind, but many sockets can connect.
 */

class Publisher: public zmqBase {
public:
    Publisher();
    // Publisher(std::string addr, bool bind=true);  // tcp://x.x.x.x:port

    // [[deprecated("use publish instead")]]
    // inline void pub(zmq::message_t& msg){publish(msg);}
    void publish(zmq::message_t& msg);
};


class Subscriber: public zmqBase {
public:
    // Subscriber();
    Subscriber(const std::string& topic={""});
    // Subscriber(std::string addr, std::string topic, bool bind=false);
    // Subscriber(std::string addr, bool bind=false);
    zmq::message_t recv(int flags=0);
    inline zmq::message_t recv_nb(){return recv(ZMQ_DONTWAIT);}
    // void setCallback(void(*callback)(zmq::message_t&));
    //
    // void(*callback)(zmq::message_t&);
};
