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


int main(void){

    gecko::init();

    Rate rate(2);

    Publisher *p = gecko::pubBindTCP("local", "bob2");
    if (p == nullptr) return 1;

    Transport<imu_t> buffer;
    vec_t a(1,2,3);
    imu_t v(a,a,a);

    while(gecko::ok()){
        // zmq::message_t msg("hello",5);
        zmq::message_t msg = buffer.pack(v);
        // cout << msg << endl;
        // gecko::log(gecko::DEBUG, "debug level\n");
        p->pub(msg);
        rate.sleep();
    }

    return 0;  // FIXME: don't see all threads joined by exit!!!
}
