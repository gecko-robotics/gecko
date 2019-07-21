/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#include <iostream>  // cout endl
#include <stdio.h>   // printf
#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>
#include <zmq.hpp>

using namespace std;

using namespace gecko;

int main(void){
    try {
        HostInfo host;
        Subscriber s;
        s.connect(zmqTCP(host.address, 9000));
        Rate r(40);

        while(true){
            zmq::message_t msg = s.recv_nb();
            // zmq::message_t msg;
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
