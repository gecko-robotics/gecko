/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#include <iostream>  // cout endl
#include <stdio.h>   // printf
#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>
// #include <zmq.hpp>

using namespace std;
using namespace gecko;

int main(int argc, char *argv[]){
    gecko::init();

    string addr;
    if (argc == 2){
        addr = argv[1];
    }
    else {
        HostInfo host;
        addr = host.address;
    }

    try {
        Subscriber s;
        s.connect(zmqTCP(addr, 9000));
        Rate r(40);

        while(gecko::ok()){
            zmq::message_t msg = s.recv_nb();
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

    printf(">> sub bye\n");

    return 0;
}
