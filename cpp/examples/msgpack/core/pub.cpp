/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#include <iostream>
#include <stdio.h>

#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>

using namespace std;


int main(int argc, char *argv[]){
    Publisher *p = nullptr;
    try {
        gecko::init();
        printf(">> post gecko::init\n");

        p = gecko::pubBindTCP("local", "test");
        if (p == nullptr) {
            printf("Couldn't get publisher\n");
            exit(1);
        }
        else printf(">> good publisher\n");

        Rate r(1);

        while(gecko::ok()){
            printf(">> loop\n");
            string s{"hello from publisher!"};
            zmq::message_t msg(static_cast<void*>(s.data()), s.size());

            p->publish(msg);
            printf(">> [PUB] sent msg\n");
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        gecko::shutdown();
        delete p;
        // exit(1);
        return 1;
    }
    catch(exception& e){
        printf("Other error\n");
        cout << e.what() << endl;
        gecko::shutdown();
        cout << "wtf1" << endl;
        delete p;
        cout << "wtf2" << endl;
        // exit(1);
        cout << "wtf3" << endl;
        return 1;
    }

    delete p;

    printf(">> pub bye ...\n");

    return 0;
}
