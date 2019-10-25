/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/

#include <iostream>  // cout endl
#include <stdio.h>   // printf
#include <gecko/gecko.hpp>
#include "msgs.pb.h"

using namespace std;
using namespace gecko;

template <class MSG>
MSG protobufUnpack(zmq::message_t& msg){
    string m(reinterpret_cast<const char*>(msg.data()), msg.size());
    MSG v;
    v.ParseFromString(m);
    return std::move(v);
}

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
        s.connect(zmqUDS("/tmp/simple-uds"));
        Rate r(40);

        while(gecko::ok()){
            zmq::message_t msg = s.recv_nb();
            if(msg.size() > 0){
                // string m(reinterpret_cast<const char*>(msg.data()), msg.size());
                // Vector v;
                // v.ParseFromString(m);
                // cout << ">> msg[" << m.size() << "]: " << m << endl;
                Vector v = protobufUnpack<Vector>(msg);
                cout << v.DebugString() << endl;
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
