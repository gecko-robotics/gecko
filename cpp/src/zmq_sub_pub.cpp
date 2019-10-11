#include "gecko/zmq_sub_pub.hpp"
#include "gecko/time.hpp"
#include "gecko/color.hpp"
#include "zmq.hpp"
#include <iostream>


using namespace std;

Publisher::Publisher(): zmqBase(ZMQ_PUB){}

void Publisher::publish(zmq::message_t& msg){
    // sock.send(msg);
    // size_t sr = sock.send(msg, zmq::send_flags::none).value_or(-1);
    sock.send(msg, zmq::send_flags::none);
    // cout << ">> publish(): " << sr << endl;
    // sock.send(zmq::buffer(msg), zmq::send_flags::none);
}

///////////////////////////////////////////////////

Subscriber::Subscriber(): zmqBase(ZMQ_SUB){}


/*
enum class recv_flags : int
{
    none = 0,
    dontwait = ZMQ_DONTWAIT
};
*/
zmq::message_t Subscriber::recv(zmq::recv_flags flags){
    zmq::message_t msg;
    // if(check()) sock.recv(&msg, flags);
    // sock.recv(&msg, flags);
    // size_t rr = sock.recv(msg, flags).value_or(-1);
    sock.recv(msg, flags);
    // zmq::detail::recv_result_t r = sock.recv(&msg, zmq::recv_flags::none); //.value_or(std::nullopt);
    // sock.recv(msg, zmq::recv_flags::none);
    // if (rr == -1) return {};
    return std::move(msg);
}
