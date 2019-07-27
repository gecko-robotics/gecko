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
#include <exception>

struct MulticastError : public std::exception {
    MulticastError(const std::string &s): msg("Multicast Error: " + s) {}
    MulticastError(): msg("Multicast Error") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

// http://www.kohala.com/start/mcast.api.txt
// http://www.kohala.com/start/
// https://rg3.name/201504241907.html
// https://www.cs.odu.edu/~cs779/spring17/lectures/multicasting.html

using MsgAddr = std::tuple<std::string, struct sockaddr_in>;
struct sockaddr_in make(const std::string& saddr, int port);
struct sockaddr_in make(int port, int iaddr);

/*
 * Multicast socket for a beacon
 *
 */
class SSocket{
public:
    SSocket();
    ~SSocket();
    void init();
    void init(
        std::string mc_addr_str, // FIXME: address
        uint16_t mc_port,
        uint8_t mc_ttl,
        bool reuse
    );
    void bind(int port, int addr);
    bool ready(long msec=500);
    MsgAddr recv();
    MsgAddr recv_nb(long msec=500);
    bool broadcast(const std::string& msg); // remove?
    bool send(const std::string& msg, struct sockaddr_in& addr); // sendto?
    bool send(const std::string& msg, const std::string& saddr, int port);

    void sockopt(int level, int name, int val);
    // void sockopt(int level, int name, const std::string& group);

    // void multicastGroup(const std::string& group);
    // void multicastLoop();
    // void timeToLive(int ttl);

protected:
    int sock;                   // socket descriptor
    // struct sockaddr_in mc_addr; // socket address structure -- why?

};
