#include <iostream>
// #include <thread>
// #include <chrono>
#include <stdio.h>
// #include <unistd.h>  // usleep
// #include <cstdarg>

#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>

using namespace std;

// const string sub_protocol = "tcp://localhost:5555";
// const string sub_protocol = "tcp://logan.local:5555";
// const string pub_protocol = "tcp://*:5555";

// void sub(void*){
//     string addr;
//     if (argc == 2){
//         addr = argv[1];
//     }
//     else {
//         HostInfo host;
//         addr = host.address;
//     }
//
//     try {
//         Subscriber s;
//         s.connect(zmqTCP(addr, 9000));
//         Rate r(40);
//
//         while(true){
//             zmq::message_t msg = s.recv_nb();
//             if(msg.size() > 0){
//                 imu_st m(msg);
//                 printf(">> msg time: %f\n", m.timestamp);
//             }
//             r.sleep();
//         }
//     }
//     catch(zmq::error_t& e) {
//         cout << e.what() << endl;
//         exit(1);
//     }
//
//     printf(">> sub bye\n");
//
//     return 0;
// }
//
// void pub(void*){
//     try {
//         HostInfo host;
//         Publisher p;
//         p.bind(zmqTCP(host.address, 9000));
//
//         Rate r(1);
//
//         while(true){
//             vec_t a(1,2,3);
//             imu_st b(a,a,a);  // new timestamp
//             zmq::message_t msg = b.pack();
//
//             p.publish(msg);
//             printf(">> [PUB] sent msg\n");
//             r.sleep();
//         }
//     }
//     catch(zmq::error_t& e) {
//         cout << e.what() << endl;
//         exit(1);
//     }
//
//     printf(">> pub bye ...\n");
//
//     return 0;
// }




int main(void){
    // setup a pre-done directory
    // dir_map_t d;
    // d["test"]["a"] = "tcp://logan.local:5000";
    // d["test"]["b"] = "tcp://logan.local:5000";
    // d["logan.local"]["a"] = "tcp://logan.local:5000";
    // d["tom.local"]["b"] = "tcp://logan.local:5000";

    // Node pub_node;
    // pub_node.run(pub);

    return 0;
}
