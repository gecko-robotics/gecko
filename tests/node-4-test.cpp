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

/*
Assertion failed: (rc == 0), function close, file
/Users/kevin/github/gecko/zmq/zmq.hpp, line 532.

zmq contex still open socket
*/
int pub(){
    gecko::init();

    Rate rate(2);

    Publisher *p = gecko::pubBindTCP("local", "bob2");
    if (p == nullptr) return 1;

    MsgPack<imu_st> buffer;
    vec_t a(1,2,3);

    while(gecko::ok()){
        // zmq::message_t msg("hello",5);
        imu_st v(a,a,a);
        zmq::message_t msg = buffer.pack(v);
        // cout << msg << endl;
        // gecko::log(gecko::DEBUG, "debug level\n");
        p->pub(msg);
        printf(">> msg sent\n");
        rate.sleep();
    }

    return 0;
}

int sub(){
    gecko::init();
    Subscriber *s = gecko::subConnectTCP("local", "bob2");
    if (s == nullptr) return 1;
    Rate r(10);
    MsgPack<imu_st> buffer;

    while(gecko::ok()){
        zmq::message_t m = s->recv_nb();
        if (m.size() > 0) {
            imu_st v = buffer.unpack(m);
            // gecko::log(gecko::DEBUG, "got message\n");
            cout << m << endl;
            v.print();
        }
        r.sleep();
    }

    delete s;

    return 0;
}

int main(void){
    int err=0;

    // err = pub();
    err = sub();

    return err;  // FIXME: don't see all threads joined by exit!!!
}
