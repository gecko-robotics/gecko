#pragma once

#include <arpa/inet.h>  // for sockaddr_in
#include <stdint.h>
#include <string>

class SSocket{
public:
    void init(std::string mc_addr_str={"224.3.29.110"}, uint16_t mc_port=11311, uint8_t mc_ttl=1);
    bool ready(int msec=500);
    std::string recv();
    bool send(std::string msg);

protected:
    int sock;                   // socket descriptor
    struct sockaddr_in mc_addr; //socket address structure

};
