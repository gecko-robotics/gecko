#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "marko/mcsocket.hpp"
#include "marko/bsocket.hpp"
#include "marko/ascii.hpp"

using namespace std;

int main(){

#if 0
    MCSocket sock("224.0.0.1", 11311);
    sock.multicastLoop(false);
    sock.timeToLive(1);
#else
    BCSocket sock(11311);
#endif

    sock.bind();
    sock.info();

    Ascii buffer;
    string msg;
    struct sockaddr_in remote;
    vector<string> vmsg = {"hi","1.2.3.4","me","12345"};

    for (int i=0; i < 5; i++){
        cout << ">> msg sent " << i+1 << " of 5" << endl;
        string s = buffer.pack(vmsg);
        sock.cast(s);
        tie(msg, remote) = sock.recv_nb();
        cout << "request: " << msg << " " << print_addr(remote) << endl;
        sleep(1);
    }
    cout << ">> Done ..." << endl;
    return 0;
}
