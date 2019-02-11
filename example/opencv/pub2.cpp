#include <iostream>
// #include <thread>
// #include <chrono>
#include <stdio.h>
#include <unistd.h>  // usleep
// #include <cstdarg>

#include "time.hpp"
#include "node.hpp"
// #include "signals.hpp"
#include "transport.hpp"

// msgpack stuff
#include <msgpack.hpp>
#include "msgs.hpp"
#include "serialization.hpp"

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void pub(bool *e){

    Transport<imu_t> buffer;
    try {
        pid_t pid = getpid();
        printf("-> pub pid: %d\n",pid);
        Publisher *p = Node::advertise("local", "a");
        if (p == nullptr){
            printf("Pubisher failed\n");
            return;
        }
        Rate r(30);

        while(*e){
            vec_t a(1,2,3);
            imu_t b(a,a,a);  // new timestamp
            zmq::message_t msg = buffer.pack(b);

            p->pub(msg);
            // printf(">> [PUB] sent msg\n");
            r.sleep();
        }
        delete p;
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }

    printf(">> pub bye ...\n");
}




int main(void){
    Node::init();

    // Node p; p.run(pub);
    // Node::wait();
    pub(&Node::ok);

    return 0;
}
