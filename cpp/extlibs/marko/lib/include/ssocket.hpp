/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/
// references that helped:
// http://www.kohala.com/start/mcast.api.txt
// http://www.kohala.com/start/
// https://rg3.name/201504241907.html
// https://www.cs.odu.edu/~cs779/spring17/lectures/multicasting.html

#pragma once

#include <arpa/inet.h>  // for sockaddr_in
// #include <stdint.h>
#include <string>
#include <map>
#include <tuple>
#include <exception>

typedef int SOCKET;
inline constexpr int INVALID_SOCKET = -1;
inline constexpr int SOCKET_ERROR = -1;
inline constexpr int NO_ERROR = 0;

// [[deprecated("use SocketError instead")]]
// __attribute__((deprecated("use SocketError instead")))
// [[deprecated]]
struct MulticastError : public std::exception {
    MulticastError(const std::string &s): msg("Multicast Error: " + s) {}
    MulticastError(): msg("Multicast Error") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

struct SocketError : public std::exception {
    SocketError(const std::string &s): msg("Soccket Error: " + s) {}
    SocketError(): msg("Socket Error") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

using MsgAddr = std::tuple<std::string, struct sockaddr_in>;
struct sockaddr_in make(const std::string& saddr, int port);
struct sockaddr_in make(int port, int iaddr);
std::string host_ip();
std::string print_addr(const struct sockaddr_in& addr);

static
std::map<int, std::string> debug_setsockopt = {
    {SO_REUSEADDR,       "SO_REUSEADDR"},
    {IP_MULTICAST_TTL,   "IP_MULTICAST_TTL"},
    {IP_MULTICAST_LOOP,  "IP_MULTICAST_LOOP"},
    {IP_ADD_MEMBERSHIP,  "IP_ADD_MEMBERSHIP"},
    {IP_DROP_MEMBERSHIP, "IP_DROP_MEMBERSHIP"}
};

/*
 * Simple UDP socket base class for query/response architecture. Not really
 * useful by itself. Look at classes that build upon it.
 */
class SSocket{
public:
    SSocket();
    ~SSocket();

    void bind(int port, int addr); // binds to address/port, enables reuse
    bool ready(long msec);  // checks to see if data available to read
    MsgAddr recv();
    MsgAddr recv_nb(long msec=900);
    bool send(const std::string& msg, struct sockaddr_in& addr);
    bool send(const std::string& msg, const std::string& saddr, int port);
    void sockopt(int level, int name, int val); // setsockopt()
    bool isEcho(const struct sockaddr_in& a); // checks if msg from 127.x.x.x

protected:
    SOCKET sock;  // socket descriptor

};
