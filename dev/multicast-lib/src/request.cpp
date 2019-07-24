#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "msocket.hpp"

using namespace std;

int main(){
    SSocket sock;
    sock.init();
    // sock.bind(11311, INADDR_ANY);

    for (int i=0; i < 5; i++){
        sock.broadcast("hi");
        // sock.recv_nb();
        sleep(1);
    }
    return 0;
}
