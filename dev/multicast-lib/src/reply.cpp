#include <iostream>
#include <stdio.h>
#include <string>

#include "mcsocket.hpp"
#include "bsocket.hpp"

using namespace std;

int main(){
    // SSocket sock;
    // MCSocket sock;
    BCSocket sock;
    // sock.init();
    sock.bind(11311, INADDR_ANY);
    sock.set_broadcast(11311, INADDR_BROADCAST);
    // sock.multicastLoop();
    // sock.multicastGroup("224.3.29.110", 11311);
    // sock.multicastLoop(false);

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
