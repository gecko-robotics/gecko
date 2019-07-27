#include "core.hpp"
#include <thread>
#include <stdio.h>
#include <string>
#include <iostream>

#include "geckocpp.hpp"
#include "msgs.hpp"

using namespace std;

string ip = "239.255.255.250";
int port = 12345;
string key = "test";

void core(void){
    Core c = Core(ip, port, key);
    c.run(1);  // 1 hz
}

void pub(int argc, char* argv[]){
    gecko::init(argc, argv);

    Rate rate(1);

    Publisher *p = gecko::advertise("bob");

    while(gecko::ok()){
        // vec_t a(1,2,3);
        // imu_t(a,a,a);
        zmq::message_t msg("hello",5);
        // cout << msg << endl;
        p->pub(msg);
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

int main(){
    // fake args
    int argc = 3;
    char* argv[] =
    {
        (char*)("./main"),
        (char*)("-c"),
        (char*)(ip.c_str()),
        (char*)("-p"),
        (char*)(to_string(port).c_str()),
        (char*)("-k"),
        (char*)(key.c_str())
    };

    thread a(core); //a.detach();
    thread b(pub, argc, argv); //b.detach();
    // thread c(sub, argc, argv); c.detach();

    // a.join();
    // b.join();
    // c.join();

    int loop = 5;
    while(loop){
        loop--;
        sleep(1);
        printf(">> Loop %d\n", loop);
    }

    a.join();
    b.join();

    return 0;
}
