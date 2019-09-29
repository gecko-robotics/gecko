#include <iostream>
#include <stdio.h>

#include <gecko/gecko.hpp>

using namespace std;


int main(void){
    gecko::init();
    try {
        HostInfo host;
        Publisher p;
        p.bind(zmqTCP(host.address, 9000));

        Rate r(1);

        while(gecko::ok()){
            string s{"hello from publisher!"};
            zmq::message_t msg(static_cast<void*>(s.data()), s.size());

            p.publish(msg);
            printf(">> [PUB] sent msg\n");
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        // exit(1);
    }

    printf(">> pub bye ...\n");

    return 0;
}
