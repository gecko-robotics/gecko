#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "mcsocket.hpp"
#include "bsocket.hpp"
#include "ascii.hpp"

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

    Ascii buffer;
    vector<string> vmsg = {"hi", "ho"};

    cout << ">> Listening" << endl;

    while(true){
        struct sockaddr_in remote;
        string msg;

        tie(msg, remote) = sock.recv();
        ascii_t mm = buffer.unpack(msg);

        printf("=> reply got %s from %s:%d\n",
            msg.c_str(),
            inet_ntoa(remote.sin_addr),
            ntohs(remote.sin_port));

        // handle echo of message
        if (mm[0] != "hi") {
            string r = buffer.pack(vmsg);
            sock.send(r, remote);
        }
    }
    return 0;
}
