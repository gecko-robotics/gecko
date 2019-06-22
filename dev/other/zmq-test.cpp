#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"

#include "gecko.hpp"
// #include <msgpack.hpp>

using namespace std;
// using std::cout;
// using std::endl;

// string path = zmqTCP("tcp://localhost", 5556);
string path = zmqUDS("/tmp/0");

void server(int t){

    // serializes this object.
    // std::vector<std::string> vec;
    // vec.push_back("Hello");
    // vec.push_back("MessagePack");

    // serialize it into simple buffer.
    // msgpack::sbuffer sbuf;
    // msgpack::pack(sbuf, vec);

    //  Prepare our context and publisher
    Publisher pub;
    pub.bind(path);

    //  Initialize random number generator
    uint16_t count = 0;
    while (1) {
        //  Send message to all subscribers
        zmq::message_t message(20);
        snprintf ((char *) message.data(), 20 , "%s %d", "33", count);
        pub.publish(message);

        zmq::message_t m1((void*)"33", 2);
        pub.publish(m1);
        
        zmq::message_t m2((void*)"test", 4);
        pub.publish(m2);

        snprintf ((char *) message.data(), 20 , "hello %d", count);
        pub.publish(message);

        count++;

        usleep(100000);

    }
}


void client(int t){
    Subscriber sub;
    sub.connect(path);

    uint16_t count, topic;
    for (int i = 0; i < 100; i++) {

        zmq::message_t msg = sub.recv();

        std::istringstream iss(static_cast<char*>(msg.data()));
        iss >> topic >> count ;
        //
        printf(">> client topic: %d  count: %d\n", topic, count);

        topic = 0;
        count = 0;

        usleep(1000000);
    }
}

int main(){
    std::cout << "start" << std::endl;
    std::thread t1(server, 500);
    std::thread t2(client, 3000);
    t1.join();
    t2.join();

    return 0;
}
