/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#include <gecko/db.hpp>
#include <gecko/gecko.hpp>
#include <gecko/msocket.hpp>
// #include <gecko/ascii.hpp>
#include <gecko/time.hpp>

#include <string>
// #include <thread>
#include <iostream>

using namespace std;

int main(){
    // setup multicast
    SSocket ss;
    string maddr = "224.3.29.110";
    uint16_t port = 11311;
    ss.init(maddr, port,1,true);

    cout << "\n==================================" << endl;
    cout << " " << maddr << ":" << port << endl;
    cout << " Sending:" << endl;

    for (int i=5; i; i--){
        cout << ">> ping" << endl;
        ss.send("hi");
        gecko::sleep(1);
    }

    return 0;
}
