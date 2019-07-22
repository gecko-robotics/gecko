#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

#include "msocket.hpp"

using namespace std;

int main(){
    SSocket sock;
    sock.init();

    for (int i=0; i < 5; i++){
        sock.send("hi");
        sock.recv_nb();
        sleep(1);
    }
    return 0;
}
