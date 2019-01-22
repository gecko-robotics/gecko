#include "core.hpp"
#include <thread>
#include <stdio.h>
#include <string>
#include <iostream>

#include "geckocpp.hpp"
#include "msgs.hpp"

using namespace std;

void core(void){
    Core c = Core();
    c.run(1);  // 1 hz
}

void pub(int argc, char* argv[]){
    gecko::init(argc, argv);

    Rate rate(1);

    Publisher *p = gecko::advertise("bob");

    while(gecko::ok()){
        vec_t a(1,2,3);
        imu_t(a,a,a);
        // zmq::message_t msg("hello",5);
        // cout << msg << endl;
        // p->pub(msg);
        rate.sleep();
    }

    // exit(0);

    delete p;
}

void callback(zmq::message_t& msg){
    // printf(">> callback: %s\n", reinterpret_cast<char*>(msg.data()));
    printf(">");
}


void sub(int argc, char* argv[]){
    gecko::init(argc, argv);

    Subscriber *s = gecko::subscribe("bob", callback);
    gecko::spin();
}

int main(int argc, char* argv[]){
    thread a(core);
    thread b(pub, argc, argv);
    thread c(sub, argc, argv);

    a.join();
    b.join();

    return 0;
}
