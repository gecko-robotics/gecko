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

using namespace std;


int main(void){
    Subscriber *s = nullptr;
    try {
        gecko::init();
        s = gecko::subConnectTCP("local", "ryan");
        if (s == nullptr) {
            printf("Couldn't get subscriber\n");
            exit(1);
        }

        Rate r(40);

        while(gecko::ok()){
            zmq::message_t msg = s->recv_nb();
            if(msg.size() > 0){
                string m(reinterpret_cast<const char*>(msg.data()), msg.size());
                printf(">> msg time: %s\n", m.c_str());
            }
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }

    delete s;

    printf(">> sub bye\n");

    return 0;
}
