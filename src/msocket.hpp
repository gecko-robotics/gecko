/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <arpa/inet.h>  // for sockaddr_in
#include <stdint.h>
#include <string>

// http://www.kohala.com/start/mcast.api.txt
// http://www.kohala.com/start/
// https://rg3.name/201504241907.html

class SSocket{
public:
    void init(
        std::string mc_addr_str={"224.3.29.110"},
        uint16_t mc_port=11311,
        uint8_t mc_ttl=1
    );
    bool ready(long msec=500);
    std::string recv(long msec=500);
    bool send(std::string msg);

protected:
    int sock;                   // socket descriptor
    struct sockaddr_in mc_addr; //socket address structure

};
