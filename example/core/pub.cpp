/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#include <iostream>
#include <stdio.h>

#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>

using namespace std;


int main(int argc, char *argv[]){
    try {
        gecko::init();
        Publisher *p = gecko::pubBindTCP("local", "test");
        if (p == nullptr) {
            printf("Couldn't get publisher\n");
            exit(1);
        }

        Rate r(1);

        while(gecko::ok()){
            vec_t a(1,2,3);
            imu_st b(a,a,a);  // new timestamp
            zmq::message_t msg = b.pack();

            p->publish(msg);
            printf(">> [PUB] sent msg\n");
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }

    printf(">> pub bye ...\n");

    return 0;
}
