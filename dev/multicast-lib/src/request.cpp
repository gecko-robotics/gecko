#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

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

    for (int i=0; i < 5; i++){
        cout << ">> msg sent " << i+1 << " of 5" << endl;
        sock.cast("hi");
        sock.recv_nb();
        sleep(1);
    }
    cout << ">> Done ..." << endl;
    return 0;
}
