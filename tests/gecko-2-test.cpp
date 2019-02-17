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

bool tcp = false;

void pubt(void *k){
    gecko::init();

    string *s = (string*)k;
    printf("Args: %s\n", s->c_str());

    Rate rate(2);

    Publisher *p;
    if(tcp) p = gecko::pubBindTCP("local", "bob2");
    else p = gecko::pubBindUDS("local", "bob2", "uds_file");

    if (p == nullptr) {
        printf("*** Subscribe failure ***\n");
        gecko::shutdown();
        return;
    }

    while(gecko::ok()){
        zmq::message_t msg("hello",5);
        p->pub(msg);
        rate.sleep();
    }

    delete p;
}

void subt(void *k){
    gecko::init();
    Subscriber *s;
    if (tcp) s = gecko::subConnectTCP("local", "bob2");
    else s = gecko::subConnectUDS("local", "bob2");

    if (s == nullptr) {
        printf("*** Subscribe failure ***\n");
        gecko::shutdown();
        return;
    }

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

    gecko::wait();

    return 0;
}
