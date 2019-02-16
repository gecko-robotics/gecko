#include "gecko.hpp"
#include <string>
#include <thread>
#include <iostream>
#include "time.h"
#include "msgs.hpp"
#include "signals.hpp"
#include "geckocpp.hpp"
#include <stdlib.h>

using namespace std;


void pubt(){
    gecko::init();

    Rate rate(2);

    Publisher *p = gecko::advertise("local", "bob2");
    if (p == nullptr) return;

    while(gecko::ok()){
        zmq::message_t msg("hello",5);
        // cout << msg << endl;
        // gecko::log(gecko::DEBUG, "debug level\n");
        p->pub(msg);
        rate.sleep();
    }

    delete p;
}

void subt(){
    gecko::init();
    Subscriber *s = gecko::subscribe("local", "bob2");
    if (s == nullptr) return;
    Rate r(10);

    while(gecko::ok()){
        zmq::message_t m = s->recv_nb();
        if (m.size() > 0) gecko::log(gecko::DEBUG, "got message\n");
        r.sleep();
    }

    delete s;
}

int main(){
    // fake args
    // int argc = 3;
    // char* argv[] =
    // {
    //     (char*)("./main"),
    //     (char*)("-c"),
    //     (char*)("239.255.255.250"),
    //     (char*)("-p"),
    //     (char*)("12345")
    // };

    // system("../../pycore/pycore.py");


    thread t1(pubt); t1.detach();
    thread t2(subt); t2.detach();

    while(gecko::ok());

    return 0;
}
