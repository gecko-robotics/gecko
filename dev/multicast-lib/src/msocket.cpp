#include "msocket.hpp"

#include <sys/types.h>  /* for type definitions */
#include <sys/socket.h> /* for socket API calls */
#include <netinet/in.h> /* for address structs */
// #include <arpa/inet.h>  /* for sockaddr_in */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for strlen() */
#include <unistd.h>     /* for close() */
#include <string>
#include <iostream>
#include <map>

using namespace std;

inline constexpr int MAX_LEN = 1024;

//
// do not bend, ha, ha, ha ...
// crease crumple cramp
// - Neuman
//

map<int, string> debug_setsockopt = {
    {SO_REUSEADDR,       "SO_REUSEADDR"},
    {IP_MULTICAST_TTL,   "IP_MULTICAST_TTL"},
    {IP_MULTICAST_LOOP,  "IP_MULTICAST_LOOP"},
    {IP_ADD_MEMBERSHIP,  "IP_ADD_MEMBERSHIP"},
    {IP_DROP_MEMBERSHIP, "IP_DROP_MEMBERSHIP"}
};

struct sockaddr_in make(const string& saddr, int port){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(saddr.c_str());
    addr.sin_port        = htons(port);
    cout << ">> Addr: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
    return std::move(addr);
}

struct sockaddr_in make(int port, int iaddr){
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(iaddr);
    return std::move(addr);
}

SSocket::~SSocket(){
    sockopt(IPPROTO_IP, IP_DROP_MEMBERSHIP, "224.3.29.110");
    ::close(sock);
}

void SSocket::init(){
    this->init("224.3.29.110", 11311, 1, true);
}

void SSocket::init(string mc_addr_str, uint16_t mc_port, uint8_t mc_ttl, bool reuse){
    // create a socket for sending to the multicast address
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        throw MulticastError("SSocket::init socket() SOCK_DGRAM failed");
    }

    // allow multiple sockets to re-use the same port
    // sockopt(SOL_SOCKET, SO_REUSEADDR, 1);

    // set multicast time to live
    // sockopt(IPPROTO_IP, IP_MULTICAST_TTL, mc_ttl);

    //
    // If you plan to have more than one process or user "listening",
    // loopback must be enabled.
    // 0-disable  1-enable
    // sockopt(IPPROTO_IP, IP_MULTICAST_LOOP, 0);

    // construct a multicast address structure
    mc_addr = make(mc_port, INADDR_ANY);

    // bind to all interfaces to receive address
    // this->bind(mc_port, INADDR_ANY);

    // request that the kernel join a multicast group
    // sockopt(IPPROTO_IP, IP_ADD_MEMBERSHIP, mc_addr_str);
}

void SSocket::bind(int port, int addr){
    struct sockaddr_in aaddr = make(port, addr);
    if (::bind(sock, (struct sockaddr*) &aaddr, sizeof(aaddr)) < 0) {
        throw MulticastError("SSock::bind failed");
    }
}

void SSocket::sockopt(int level, int name, int val){
    int err = setsockopt(sock, level, name, (int*) &val, sizeof(val));
    if (err < 0){
        throw MulticastError("SSocket::setsockopt " + debug_setsockopt[name] + " failed");
    }
}

void SSocket::sockopt(int level, int name, const string& group){
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    int err = setsockopt(sock, level, name, (char*) &mreq, sizeof(mreq));
    if (err < 0){
        throw MulticastError("SSock::setsockopt " + debug_setsockopt[name] + " failed");
    }
}

bool SSocket::ready(long msec){
    struct timeval tv;
    fd_set readfds;
    long sec = 0;

    if (msec >= 1000) {
        sec = msec/1000;
        msec %= 1000;
        // printf("time %ld %ld\n",sec, msec);
    }

    tv.tv_sec = sec;
    tv.tv_usec = msec * 1000;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    // don't care about writefds and exceptfds:
    if (select(sock+1, &readfds, NULL, NULL, &tv) < 0)
        throw MulticastError("SSocket::ready select issue");

    if (FD_ISSET(sock, &readfds)) return true;
    return false;
}

MsgAddr SSocket::recv(){
    // clear the receive buffers & structs
    char recv_str[MAX_LEN];
    memset(recv_str, 0, sizeof(recv_str));

    struct sockaddr_in from_addr;
    unsigned int from_len = sizeof(from_addr);
    memset(&from_addr, 0, sizeof(from_addr));

    // block waiting to receive a packet
    if ((recvfrom(sock, recv_str, MAX_LEN, 0,
         (struct sockaddr*)&from_addr, &from_len)) < 0) {
        throw MulticastError("recvfrom() failed");
    }

    /* output received string */
    // printf("=> Received %d bytes from %s:%d\n",
    //     recv_len,
    //     inet_ntoa(from_addr.sin_addr),
    //     ntohs(from_addr.sin_port));

    string msg = recv_str;
    // cout << msg << endl;

    return std::move(MsgAddr(msg, from_addr));
}

MsgAddr SSocket::recv_nb(long msec){
    if(ready(msec))
        return this->recv();

    MsgAddr r;
    return r;
}

bool SSocket::broadcast(const std::string& msg){
    // cout << "=> send: " << msg << " to " << inet_ntoa(mc_addr.sin_addr) << ":" << ntohs(mc_addr.sin_port) << endl;

    // send string to multicast address
    if ((sendto(sock, msg.c_str(), msg.size(), 0,
            (struct sockaddr *) &mc_addr, sizeof(mc_addr))) != msg.size()) {
        throw MulticastError("sendto() sent incorrect number of bytes");
    }
    return true;
}

bool SSocket::send(const std::string& msg, struct sockaddr_in& addr){
    // cout << "=> send: " << msg << " to " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;

    if ((sendto(sock, msg.c_str(), msg.size(), 0,
            (struct sockaddr *) &addr, sizeof(addr))) != msg.size()) {
        throw MulticastError("sendto() sent incorrect number of bytes");
    }

    return true;
}

bool SSocket::send(const std::string& msg, const string& saddr, int port){
    // cout << "=> send: " << msg << " to " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;

    struct sockaddr_in addr = make(saddr, port);

    if ((sendto(sock, msg.c_str(), msg.size(), 0,
            (struct sockaddr *) &addr, sizeof(addr))) != msg.size()) {
        throw MulticastError("sendto() sent incorrect number of bytes");
    }

    return true;
}
