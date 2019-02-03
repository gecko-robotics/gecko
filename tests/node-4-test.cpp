#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <unistd.h>  // usleep
#include <cstdarg>

#include "time.hpp"
#include "node.hpp"
#include "signals.hpp"
#include "transport.hpp"

// msgpack stuff
#include <msgpack.hpp>
#include "msgs.hpp"
#include "serialization.hpp"

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"
#include "zmq_addon.hpp"

using namespace std;

const string sub_protocol = "tcp://localhost:5555";
const string pub_protocol = "tcp://*:5555";

void sub(bool *e){
    try {
        Subscriber s(sub_protocol);

        Rate r(3);
        Transport<imu_t> buffer;

        while(*e){
            zmq::message_t msg = s.recv_nb();
            if(msg.size() > 0){
                imu_t m = buffer.unpack(msg);
                printf(">> msg time: %f\n", m.timestamp);
            }
            else printf("** No data found\n");
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }

    printf(">> sub bye\n");
}

void pub(bool *e){

    Transport<imu_t> buffer;
    try {
        Publisher p(pub_protocol, true);
        Rate r(1);

        while(*e){
            vec_t a(1,2,3);
            imu_t b(a,a,a);  // new timestamp
            zmq::message_t msg = buffer.pack(b);

            p.pub(msg);
            printf(">> [PUB] sent msg\n");
            r.sleep();
        }
    }

    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }
    printf(">> pub bye\n");
}




int main(void){
    Node s; s.run(sub);
    Node p; p.run(pub);

    Node *n = nullptr;
    for (int i=5;i;i--) {
        n = new Node();
        n->run(sub);
    }

    Node::wait();

    return 0;  // FIXME: don't see all threads joined by exit!!!
}
