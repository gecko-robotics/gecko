/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <arpa/inet.h>  // for sockaddr_in
#include <stdint.h>
#include <string>
#include <tuple>

// http://www.kohala.com/start/mcast.api.txt
// http://www.kohala.com/start/
// https://rg3.name/201504241907.html


// class MsgAddr {
// public:
//     MsgAddr(const std::string& m, const struct sockaddr_in& a): msg(m), addr(a) {}
//     std::string msg;
//     struct sockaddr_in addr;
// };

using MsgAddr = std::tuple<std::string, struct sockaddr_in>;

/*
 * Multicast socket for a beacon
 *
 */
class SSocket{
public:
    void init(
        std::string mc_addr_str={"224.3.29.110"},
        uint16_t mc_port=11311,
        uint8_t mc_ttl=1,
        bool reuse=false
    );
    bool ready(long msec=500);
    // MsgAddr recv2();
    MsgAddr recv();
    MsgAddr recv_nb(long msec=500);
    // std::string recv();
    // std::string recv_nb(long msec=500);
    bool send(const std::string& msg); // remove?
    bool send(const std::string& msg, struct sockaddr_in& addr); // sendto?

// protected:
    int sock;                   // socket descriptor
    struct sockaddr_in mc_addr; // socket address structure
    struct sockaddr_in from_addr;

};
