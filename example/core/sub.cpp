/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#include <iostream>  // cout endl
#include <stdio.h>   // printf
#include <unistd.h>  // usleep
// #include <cstdarg>

#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>

using namespace std;


int main(void){
    try {
        gecko::init();
        Subscriber *s = gecko::subConnectTCP("local", "test");
        if (s == nullptr) {
            printf("Couldn't get subscriber\n");
            exit(1);
        }

        Rate r(40);

        while(true){
            zmq::message_t msg = s->recv_nb();
            if(msg.size() > 0){
                imu_st m(msg);
                printf(">> msg time: %f\n", m.timestamp);
            }
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }

    printf(">> sub bye\n");

    return 0;
}
