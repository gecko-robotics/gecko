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

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"
#include "zmq_addon.hpp"

using namespace std;
//
// const string sub_protocol = "tcp://localhost:5555";
// const string pub_protocol = "tcp://*:5555";
//
// void sub(bool *e){
//     try {
//         Subscriber s(sub_protocol);
//
//         Rate r(3);
//
//         while(*e){
//             zmq::message_t msg = s.recv_nb();
//             if(msg.size() > 0){
//                 string s(static_cast<char*>(msg.data()), msg.size());
//                 printf(">> [SUB] msg[%zu]: %s\n", s.size(), s.c_str());
//             }
//             else printf("** No data found\n");
//             r.sleep();
//         }
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
//     // zmq::context_t context(1);
//     // const string protocol = "tcp://*:5555";
//     // zmq::socket_t sock (context, ZMQ_PUB);
//     // sock.bind(protocol);
//     try {
//         Publisher p(pub_protocol, true);
//         Rate r(1);
//
//         while(*e){
//             zmq::message_t msg((void*)"kevin", 5);
//             // sock.send(msg);
//             p.pub(msg);
//             printf(">> [PUB] sent msg\n");
//             r.sleep();
//         }
//     }
//
//     catch(zmq::error_t& e) {
//         cout << e.what() << endl;
//         exit(1);
//     }
//     // sock.close();
//     printf(">> pub bye\n");
// }
//
//
int main(void){
//     Node s; s.run(sub);
//     Node p; p.run(pub);
//
//     Node *n = nullptr;
//     for (int i=5;i;i--) {
//         n = new Node();
//         n->run(sub);
//     }
//
//     Node::wait();
//
    return 0;  // FIXME: don't see all threads joined by exit!!!
}
