#include <msgpack.hpp>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include "msgpack_pub_sub.hpp"
#include "msgs.hpp"
#include "gecko.hpp"

using namespace std;

int main(){
    Rate rate(1);
    Publisher *p = new Publisher(zmqUDS("/tmp/0")); // gecko::pubBindUDS()
    mpPublisher<vec_t> pub(p);

    while(gecko::ok()) {
        vec_t a(10,200,-30);
        pub.publish(a);
        cout << "pub" << endl;
        rate.sleep();
    }

    return 0;
}
