#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "mcsocket.hpp"

using namespace std;

int main(){
    // SSocket sock;
    MCSocket sock;
    sock.init();
    sock.bind(11311, INADDR_ANY);
    sock.multicastLoop();
    sock.multicastGroup("224.3.29.110", 11311);
    // sock.timeToLive(32);

    for (int i=0; i < 5; i++){
        cout << ">> msg sent " << i+1 << " of 5" << endl;
        sock.broadcast("hi");
        sock.recv_nb();
        sleep(1);
    }
    cout << ">> Done ..." << endl;
    return 0;
}
