#include <iostream>
#include <stdio.h>

#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>
// #include <gecko/geckocpp.hpp>

using namespace std;


int main(int argc, char *argv[]){
    gecko::init();

    // for testing, I am going to create the data file here so I don't
    // have to find it later when I read the data file
    //BONUS: I am checking both read/write
    json j = {
        {"topic a", {"localhost", 9000}},
        {"topic b", {"localhost", 9001}}
    };

    writeJson("robot.json", j);

    json dict;
    readJson("robot.json", dict);

    cout << dict << endl;

    for (const auto& [k,v]: dict.items()) cout << k << " => " << v << endl;

    string addr;
    string a = dict["topic a"][0];
    int port = dict["topic a"][1];

    if (a == "localhost"){
        HostInfo host;
        addr = host.address;
    }
    else
        addr = a;

    try {
        Publisher p;
        p.bind(zmqTCP(addr, port));

        Rate r(1);

        while(gecko::ok()){
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
        // exit(1);
    }

    printf(">> pub bye ...\n");

    return 0;
}
