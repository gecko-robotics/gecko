/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#include <iostream>  // cout endl
#include <stdio.h>   // printf
#include <unistd.h>  // usleep
// #include <cstdarg>

#include "gecko.hpp"

using namespace std;

void sub(bool *e){
    try {
        pid_t pid = getpid();
        printf("-> sub pid: %d\n",pid);
        Subscriber *s = Node::subscribe("local","a");
        if (s == nullptr) {
            printf("Subscription failed\n");
            return;
        }

        Rate r(40);
        Transport<imu_t> buffer;

        while(*e){
            // if (s->check())
            if (1)
            {
                zmq::message_t msg = s->recv_nb();
                if(msg.size() > 0){
                    imu_t m = buffer.unpack(msg);
                    printf(">> msg time: %f\n", m.timestamp);
                }
            }
            // else printf("** No data found\n");
            r.sleep();
        }
        delete s;
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }

    printf(">> sub bye\n");
}

int main(void){
    Node::init();

    Node *n = nullptr;
    for (int i=5;i>0;i--) {
        n = new Node();
        n->run(sub);
        printf("-> thread[%d]: %p\n",i,&n);
    }

    Node::wait();

    return 0;
}
