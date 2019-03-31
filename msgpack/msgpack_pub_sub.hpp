/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <msgs.hpp>
#include <zmq.hpp>
#include <transport.hpp>


template<class T>
class mpPublisher {
public:

    mpPublisher(Publisher *p): pub(p) {};

    void publish(const T& msg){
        zmq::message_t m = buffer.pack(msg);
        pub->pub(m);
    }

protected:
    Publisher *pub;
    MsgPack<T> buffer;
};

template<class T>
class mpSubscriber {
public:

    mpSubscriber(Subscriber *s): sub(s) {};

    T recv(){
        zmq::message_t m = sub->recv();
        T msg = buffer.unpack(m);
        return msg;
    }

    T recv_nb(){
        zmq::message_t m = sub->recv_nb();

        if (m.size() == 0) return T();

        T msg = buffer.unpack(m);
        return msg;
    }

protected:
    Subscriber *sub;
    MsgPack<T> buffer;
};
