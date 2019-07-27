#include <iostream>
#include <stdio.h>
#include <string>

#include "mcsocket.hpp"
#include "bsocket.hpp"

using namespace std;

int main(){

#if 0
    MCSocket sock("224.3.29.110", 11311);
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

        printf("=> Received %s from %s:%d\n",
            msg.c_str(),
            inet_ntoa(remote.sin_addr),
            ntohs(remote.sin_port));

        // handle echo of message
        if (msg != "hellooo!") sock.send("hellooo!", remote);
    }
    return 0;
}
