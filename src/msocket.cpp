#include "gecko/msocket.hpp"

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

using namespace std;

void SSocket::init(string mc_addr_str, uint16_t mc_port, uint8_t mc_ttl, bool reuse){
    // create a socket for sending to the multicast address
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        // perror("SSocket::init socket() SOCK_DGRAM failed");
        // exit(1);
        throw MulticastError("SSocket::init socket() SOCK_DGRAM failed");
    }

    if (reuse) {
        u_int yes = 1;
        int err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes));
        if (err < 0){
            // perror("SSocket::init SO_REUSE failed");
            // // return 1;
            // exit(1);
            throw MulticastError("SSocket::init SO_REUSE failed");
        }
    }

    // set the TTL (time to live/hop count) for the send
    if ((setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
       (void*) &mc_ttl, sizeof(mc_ttl))) < 0) {
        // perror("SSocket::init setsockopt() IP_MULTICAST_TTL failed");
        // exit(1);
        throw MulticastError("SSocket::init setsockopt() IP_MULTICAST_TTL failed");
    }

    //
    // self.sock.setsockopt(socket.SOL_IP, socket.IP_MULTICAST_LOOP, 1)
    // If you plan to have more than one process or user "listening",
    // loopback must be enabled.
    u_char one = 0;  // 0-disable  1-enable
    if ((setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP,
       (void*) &one, sizeof(one))) < 0) {
        // perror("SSocket::init setsockopt() IP_MULTICAST_TTL failed");
        // exit(1);
        throw MulticastError("SSocket::init setsockopt() IP_MULTICAST_TTL failed");
    }

    // construct a multicast address structure
    memset(&mc_addr, 0, sizeof(mc_addr));
    mc_addr.sin_family      = AF_INET;
    mc_addr.sin_addr.s_addr = inet_addr(mc_addr_str.c_str());
    mc_addr.sin_port        = htons(mc_port);

    // bind to all interfaces to receive address
    struct sockaddr_in aaddr;
    memset(&aaddr, 0, sizeof(aaddr));
    aaddr.sin_family = AF_INET;
    aaddr.sin_addr.s_addr = inet_addr("0.0.0.0"); // need to send response back
    aaddr.sin_port = htons(mc_port);
    if (::bind(sock, (struct sockaddr*) &aaddr, sizeof(aaddr)) < 0) {
        // perror("Listener::listen() --> bind");
        // return 1;
        throw MulticastError("Listener::listen() --> bind");
    }

    // use setsockopt() to request that the kernel join a multicast group
    struct ip_mreq mreq;
    string group = mc_addr_str;
    mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    int err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
    if (err < 0){
        // perror("Listener::listen() --> setsockopt");
        // return 1;
        throw MulticastError("Listener::listen() --> setsockopt");
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
        // perror("SSocket::ready select issue");
        throw MulticastError("SSocket::ready select issue");

    if (FD_ISSET(sock, &readfds)) return true;
    return false;
}

MsgAddr SSocket::recv(){
    uint16_t recv_len = 0;
    const uint16_t MAX_LEN = 1024;
    unsigned int from_len;
    char recv_str[1024];
    struct sockaddr_in from_addr;
    std::string msg;

    /* clear the receive buffers & structs */
    memset(recv_str, 0, sizeof(recv_str));
    from_len = sizeof(from_addr);
    memset(&from_addr, 0, sizeof(from_addr));

    /* block waiting to receive a packet */
    if ((recv_len = recvfrom(sock, recv_str, MAX_LEN, 0,
         (struct sockaddr*)&from_addr, &from_len)) < 0) {
        // perror("recvfrom() failed");
        // exit(1);
        throw MulticastError("recvfrom() failed");
    }

    /* output received string */
    // printf("=> Received %d bytes from %s:%d\n",
    //     recv_len,
    //     inet_ntoa(from_addr.sin_addr),
    //     ntohs(from_addr.sin_port));

    msg = recv_str;
    // cout << msg << endl;

    return MsgAddr(msg, from_addr);
}

MsgAddr SSocket::recv_nb(long msec){
    if(ready(msec))
        return this->recv();

    MsgAddr r;
    return r;
}

bool SSocket::send(const std::string& msg){
    // cout << "=> send: " << msg << " to " << inet_ntoa(mc_addr.sin_addr) << ":" << ntohs(mc_addr.sin_port) << endl;
    /* send string to multicast address */
    if ((sendto(sock, msg.c_str(), msg.size(), 0,
            (struct sockaddr *) &mc_addr, sizeof(mc_addr))) != msg.size()) {
        // perror("sendto() sent incorrect number of bytes");
        // // exit(1);
        // return false;
        throw MulticastError("sendto() sent incorrect number of bytes");
    }
    return true;
}

bool SSocket::send(const std::string& msg, struct sockaddr_in& addr){
    // cout << "=> send: " << msg << " to " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;

    if ((sendto(sock, msg.c_str(), msg.size(), 0,
            (struct sockaddr *) &addr, sizeof(addr))) != msg.size()) {
        // perror("sendto() sent incorrect number of bytes");
        // // exit(1);
        // return false;
        throw MulticastError("sendto() sent incorrect number of bytes");
    }

    // sleep(1);
    return true;
}
