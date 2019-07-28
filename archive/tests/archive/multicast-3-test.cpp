#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <unistd.h>  // usleep
#include <sstream>

#include "time.hpp"
#include "event.hpp"
#include "signals.hpp"
#include "network.hpp"

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"
#include "zmq_addon.hpp"

using namespace std;

// void sub(bool *e){
//     zmq::context_t context(1);
//     const string protocol = "tcp://localhost:5555";
//     zmq::socket_t sock (context, ZMQ_SUB);
//     sock.connect(protocol);
//     sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);
//
//     Rate r(10);
//
//     while(*e){
//         zmq::message_t msg;
//         sock.recv(&msg, ZMQ_DONTWAIT); // non-blocking
//         // sock.recv(&msg, 0); // blocking
//         if(msg.size() > 0){
//             string s(static_cast<char*>(msg.data()), msg.size());
//             printf(">> [SUB] msg[%zu]: %s\n", s.size(), s.c_str());
//         }
//         else printf("** No data found\n");
//         // usleep(100000);
//         r.sleep();
//     }
//     sock.close();
//     printf(">> sub bye\n");
// }

// void pub(bool *e){
//     zmq::context_t context(1);
//     const string protocol = "tcp://*:5555";
//     zmq::socket_t sock (context, ZMQ_PUB);
//     sock.bind(protocol);
//     Rate r(1);
//
//     while(*e){
//         zmq::message_t msg((void*)"kevin", 5);
//         sock.send(msg);
//         printf(">> [PUB] sent msg\n");
//         r.sleep();
//     }
//     sock.close();
//     printf(">> pub bye\n");
// }

void pub(bool *e){
//     try {
//     zmq::context_t context(1);
//     HostInfo h = HostInfo();
//     stringstream ss;
//     // ss << "norm://" << h.address << ";239.192.1.1:5555";
//     ss << "pgm://224.1.2.3:5556";
//     zmq::socket_t sock (context, ZMQ_PUB);
//     sock.bind(ss.str());
//     Rate r(1);
//
//     while(*e){
//         zmq::message_t msg((void*)"kevin", 5);
//         sock.send(msg);
//         printf(">> [PUB] sent msg\n");
//         r.sleep();
//     }
//     sock.close();
//     printf(">> pub bye\n");
// }
//     catch(zmq::error_t& e) {
//         cout << e.what() << endl;
//         exit(1);
//     }
}

int main(void){
    SigCapture sig;

    // thread s(sub, &sig.ok); s.detach();
    thread p(pub, &sig.ok); p.detach();

    while(sig.ok){sleep(1);}
    sleep(2);

    return 0;
}
