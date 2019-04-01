#pragma once

#include "transport.hpp"
#include <stdint.h>
#include <string>
#include "zmq.hpp"

class Publisher: public zmqBase {
public:
    Publisher();
    // Publisher(std::string addr, bool bind=true);  // tcp://x.x.x.x:port

    [[deprecated("use publish instead")]]
    inline void pub(zmq::message_t& msg){publish(msg);}
    void publish(zmq::message_t& msg);

protected:

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
