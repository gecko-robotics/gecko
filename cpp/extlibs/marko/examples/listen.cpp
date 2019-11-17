#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "marko/mcsocket.hpp"
#include "marko/bsocket.hpp"
#include "marko/ascii.hpp"

using namespace std;

int main(){

#if 1
    MCSocket sock("224.0.0.1", 11311);
    sock.multicastLoop(false);
    sock.timeToLive(1);
#else
    BCSocket sock(11311);
#endif

    sock.bind();
    sock.info();

    cout << ">> Listening" << endl;

    while(true){
        struct sockaddr_in remote;
        string msg;

        tie(msg, remote) = sock.recv();

        printf("=> reply got %s from %s:%d\n",
            msg.c_str(),
            inet_ntoa(remote.sin_addr),
            ntohs(remote.sin_port));

        // handle echo of message
        // if (mm[0] != "hi") {
        //     string r = buffer.pack(vmsg);
        //     sock.send(r, remote);
        // }
    }
    return 0;
}
