#include <iostream>
#include <stdio.h>
#include <string>

#include "msocket.hpp"

using namespace std;

int main(){
    SSocket sock;
    sock.init();

    while(true){
        struct sockaddr_in remote;
        string s;

        tie(s, remote) = sock.recv();

        cout << "Reply: " << s << endl;

        sock.send("hellooo!", remote);
    }
    return 0;
}
