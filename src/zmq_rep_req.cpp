#include "gecko/zmq_rep_req.hpp"
#include "gecko/time.hpp"
#include "gecko/color.hpp"
#include "zmq.hpp"
#include <iostream>

using namespace std;


Reply::Reply(std::string addr): zmqBase(ZMQ_REP){
    sock.bind(addr);
    setEndPt();
}

void Reply::listen(zmq::message_t (*callback)(zmq::message_t&), int flags){
    zmq::message_t request;

    if (zmqBase::check(1) == false) return;

    sock.recv (&request, flags);

    // if (request.size() == 0) return;

    //  create the reply
    zmq::message_t reply = callback(request);
    printf(">> sending reply\n");
    // cout << reply << endl;
    if (reply.size() > 0) sock.send(reply, zmq::send_flags::none);
}


////////////////////////////////////////////////////

Request::Request(std::string addr): zmqBase(ZMQ_REQ){
    sock.connect(addr);
    setEndPt();
}

zmq::message_t Request::get(zmq::message_t& msg, int flags){
    sock.send(msg);

    bool msg_ready = zmqBase::check(1);

    zmq::message_t rep;
    if (msg_ready) sock.recv(&rep, flags);

    return rep;
}
