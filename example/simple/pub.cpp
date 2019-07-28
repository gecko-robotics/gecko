#include <iostream>
#include <stdio.h>

#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>

using namespace std;


int main(void){
    try {
        HostInfo host;
        Publisher p;
        p.bind(zmqTCP(host.address, 9000));

        Rate r(1);

        while(true){
            vec_t a(1,2,3);
            imu_st b(a,a,a);  // new timestamp
            zmq::message_t msg = b.pack();

            p.publish(msg);
            printf(">> [PUB] sent msg\n");
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        exit(1);
    }

    printf(">> pub bye ...\n");

    return 0;
}
