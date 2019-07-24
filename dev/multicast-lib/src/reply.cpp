#include <iostream>
#include <stdio.h>
#include <string>

#include "msocket.hpp"

using namespace std;

int main(){
    SSocket sock;
    sock.init();
    sock.bind(11311, INADDR_ANY);
    // sock.sockopt(IPPROTO_IP, IP_ADD_MEMBERSHIP, "224.3.29.110");
    sock.multicastGroup("224.3.29.110");

    while(true){
        struct sockaddr_in remote;
        string s;

        tie(s, remote) = sock.recv();

        cout << "Reply: " << s << endl;

        // sock.send("hellooo!", remote);
    }
    return 0;
}
