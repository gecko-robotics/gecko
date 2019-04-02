/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <msgs.hpp>
#include <zmq.hpp>
#include <transport.hpp>
#include "zmq_sub_pub.hpp"
#include "zmq_rep_req.hpp"


/***
 * Have publisher already bind/connect, this just wraps the publish
 */
template<class T>
class mpPublisher {
public:

    mpPublisher(Publisher *p): pub(p) {};
    ~mpPublisher(){
        if (pub == nullptr) return;
        pub->close();
    }

    void publish(const T& msg){
        zmq::message_t m = buffer.pack(msg);
        pub->publish(m);
    }

protected:
    Publisher *pub;
    MsgPack<T> buffer;
};


/***
 * Have subscriber already bind/connect, this just wraps the recv(_nb)
 */
template<class T>
class mpSubscriber {
public:

    mpSubscriber(Subscriber *s): sub(s) {};
    ~mpSubscriber(){
        if (sub == nullptr) return;
        sub->close();
    }

    bool recv(T& msg){
        zmq::message_t m = sub->recv();
        msg = buffer.unpack(m);
        return true;
    }

    bool recv_nb(T& msg){
        zmq::message_t m = sub->recv_nb();

        if (m.size() == 0) return false;

        msg = buffer.unpack(m);
        return true;
    }

protected:
    Subscriber *sub;
    MsgPack<T> buffer;
};
