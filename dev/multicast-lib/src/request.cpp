#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "msocket.hpp"

using namespace std;

int main(){
    SSocket sock;
    sock.init();
    sock.bind(11311, INADDR_ANY);
    // sock.multicastLoop();
    sock.multicastGroup("224.3.29.110");
    // sock.timeToLive(32);

    for (int i=0; i < 5; i++){
        sock.broadcast("hi");
        sock.recv_nb();
        sleep(1);
    }
    return 0;
}
