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

// const string sub_protocol = "tcp://localhost:5555";
// const string sub_protocol = "tcp://logan.local:5555";
// const string pub_protocol = "tcp://*:5555";

// void sub(bool *e){
//     try {
//         pid_t pid = getpid();
//         printf("-> sub pid: %d\n",pid);
//         Subscriber *s = Node::subscribe("local","a");
//         if (s == nullptr) {
//             printf("Subscription failed\n");
//             return;
//         }
//
//         Rate r(3);
//         Transport<imu_t> buffer;
//
//         while(*e){
//             zmq::message_t msg = s->recv_nb();
//             if(msg.size() > 0){
//                 imu_t m = buffer.unpack(msg);
//                 printf(">> msg time: %f\n", m.timestamp);
//             }
//             // else printf("** No data found\n");
//             r.sleep();
//         }
//         delete s;
//     }
//     catch(zmq::error_t& e) {
//         cout << e.what() << endl;
//         exit(1);
//     }
//
//     printf(">> sub bye\n");
// }
//
// void pub(bool *e){
//
//     Transport<imu_t> buffer;
//     try {
//         pid_t pid = getpid();
//         printf("-> pub pid: %d\n",pid);
//         Publisher *p = Node::advertise("local", "a");
//         if (p == nullptr){
//             printf("Pubisher failed\n");
//             return;
//         }
//         Rate r(1);
//
//         while(*e){
//             vec_t a(1,2,3);
//             imu_t b(a,a,a);  // new timestamp
//             zmq::message_t msg = buffer.pack(b);
//
//             p->pub(msg);
//             printf(">> [PUB] sent msg\n");
//             r.sleep();
//         }
//         delete p;
//     }
//     catch(zmq::error_t& e) {
//         cout << e.what() << endl;
//         exit(1);
//     }
//
//     printf(">> pub bye\n");
// }




int main(void){
    // setup a pre-done directory
    // dir_map_t d;
    // d["test"]["a"] = "tcp://logan.local:5000";
    // d["test"]["b"] = "tcp://logan.local:5000";
    // d["logan.local"]["a"] = "tcp://logan.local:5000";
    // d["tom.local"]["b"] = "tcp://logan.local:5000";

    // Node::init(d);
    // Node::init();
    //
    // Node p; p.run(pub);
    // printf("sleep\n");
    // sleep(2);
    // printf("awake\n");
    //
    // Node *n = nullptr;
    // for (int i=5;i>0;i--) {
    //     n = new Node();
    //     n->run(sub);
    //     printf("-> thread[%d]: %p\n",i,&n);
    // }
    //
    // Node::wait(5);

    return 0;
}
