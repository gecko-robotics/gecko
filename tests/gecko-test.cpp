#include "gecko.hpp"
#include <string>
#include <thread>
#include <iostream>
#include "time.h"
#include "msgs.hpp"

using namespace std;


void pubt(int argc, char* argv[]){
    gecko::init(argc, argv);

    Rate rate(2);

    Publisher *p = gecko::advertise("bob");

    while(gecko::ok()){
        zmq::message_t msg("hello",5);
        cout << msg << endl;
        p->pub(msg);
        rate.sleep();
    }

    delete p;
}

void callback(zmq::message_t& m){
    cout << m << endl;
}

void subt(int argc, char* argv[]){
    gecko::init(argc, argv);
    // gecko::subscribe("bob", callback);
    // gecko::spin();
}

int main(){
    // fake args
    int argc = 3;
    char* argv[] =
    {
        (char*)("./main"),
        (char*)("-c"),
        (char*)("239.255.255.250"),
        (char*)("-p"),
        (char*)("12345")
    };

    thread t1(pubt, argc, argv); t1.detach();
    // thread t2(subt, argc, argv);

    // t1.join();
    // t2.join();

    while(1);

    return 0;
}
