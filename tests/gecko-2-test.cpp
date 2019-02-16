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


void pubt(void *k){
    gecko::init();

    string *s = (string*)k;
    printf("Args: %s\n", s->c_str());

    Rate rate(2);

    Publisher *p = gecko::advertise("local", "bob2");
    if (p == nullptr) return;

    while(gecko::ok()){
        zmq::message_t msg("hello",5);
        p->pub(msg);
        rate.sleep();
    }

    delete p;
}

void subt(void *k){
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

    string *tmp = new string("hello");

    Node p; p.run(pubt, (void*)tmp);
    Node s; s.run(subt);

    while(gecko::ok());

    return 0;
}
