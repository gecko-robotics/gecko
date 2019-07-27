#include <iostream>
#include <stdio.h>
#include <string>

#include "mcsocket.hpp"
#include "bsocket.hpp"

using namespace std;

int main(){
    // MCSocket sock("224.3.29.110", 11311);
    BCSocket sock(11311);
    sock.bind();
    // sock.multicastLoop(false);
    // sock.timeToLive(32);

    sock.info();

    cout << ">> Listening" << endl;

    while(true){
        struct sockaddr_in remote;
        string msg;

        tie(msg, remote) = sock.recv();

        // cout << "Reply: " << s << endl;
        printf("=> Received %s from %s:%d\n",
            msg.c_str(),
            inet_ntoa(remote.sin_addr),
            ntohs(remote.sin_port));

        if (msg != "hellooo!") sock.send("hellooo!", remote);
    }
    return 0;
}
