#include <iostream>
#include <stdio.h>
#include <string>

#include "msocket.hpp"

using namespace std;

int main(){
    SSocket sock;
    sock.init();
    sock.bind(11311, INADDR_ANY);
    // sock.multicastLoop();
    sock.multicastGroup("224.3.29.110");

    while(true){
        struct sockaddr_in remote;
        string s;

        tie(s, remote) = sock.recv();

        // cout << "Reply: " << s << endl;
        printf("=> Received %s from %s:%d\n",
            s.c_str(),
            inet_ntoa(remote.sin_addr),
            ntohs(remote.sin_port));

        if (s != "hellooo!") sock.send("hellooo!", remote);
    }
    return 0;
}
