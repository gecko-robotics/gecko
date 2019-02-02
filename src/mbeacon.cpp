// C
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for sleep(), close()
#include <stdio.h>
#include <stdlib.h>

// C++
// #include <chrono>
#include <string>
#include <iostream>

#include "mbeacon.hpp"
#include "time.hpp"

// ?
const bool BOOL_SUCCESS = true;
const bool BOOL_FAIL = false;

using namespace std;


bool Parser::parse(const string& str, vector<string>& toks){
    stringstream ss(str);
    string token;
    try{
        while(getline(ss,token,sep)) toks.push_back(token);
    }
    catch(exception& e){
        cout << e.what() << endl;
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////

Beacon::Beacon(): MSGBUFSIZE(255), fd(0) {}
Beacon::~Beacon() {
    ::close(fd);
    printf(">> Beacon shutdown\n");
}

bool Beacon::initSocket(string group, int port, bool reuse){

    // create what looks like an ordinary UDP socket
    // fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("Beacon::initSocket");
        return true;
    }

    // allow multiple sockets to use the same PORT number
    if (reuse){
        int yes = 1;
        // u_char yes = 1;
        // int err = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes));
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0){
           perror("Reusing ADDR failed");
           return true;
        }
        else printf("SO_REUSEADDR\n");
    }
    // int yes = 1;
    // // u_char yes = 1;
    // if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0){
    //    perror("Reusing ADDR failed");
    //    return true;
    // }

    u_char ttl = 1;
    // u_char ttl = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("setsockopt() failed");
        // freeaddrinfo(*multicastAddr);
        return true;
    }
    else printf("IP_MULTICAST_TTL\n");

    // enable loopback so you don't hear your own multicast
    u_char loopch = 1;
    if(setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopch, sizeof(loopch)) < 0) {
        perror("Setting IP_MULTICAST_LOOP error");
        return true;
    }
    else printf("IP_MULTICAST_LOOP\n");

    // char s[256];
    // inet_ntop(AF_INET, &(addr.sin_addr),s, 256);
    // printf("\n\nreverse: %s\n\n", s);

    // set up destination address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group.c_str());
    // inet_pton(AF_INET, group.c_str(), &(addr.sin_addr));
    addr.sin_port = htons(port);

    // struct ip_mreq mreq;
    // mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
    // // mreq.imr_interface.s_addr = inet_addr("10.0.1.37");
    // mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    // // int err = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
    // if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
    //     perror("Beacon::initSocket() --> setsockopt(IP_ADD_MEMBERSHIP)");
    //     return true;
    // }

    return false;
}

bool Beacon::ipAddMembership(bool join){
    // int opt = (join ? IP_ADD_MEMBERSHIP : IP_DROP_MEMBERSHIP);
    // // use setsockopt() to request that the kernel join a multicast group
    // struct ip_mreq mreq;
    // mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
    // // mreq.imr_interface.s_addr = inet_addr("10.0.1.37");
    // mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    // // int err = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
    // if (setsockopt(fd, IPPROTO_IP, opt, &mreq, sizeof(mreq)) < 0){
    //     perror("BeaconServer::listen() --> setsockopt(IP_ADD_MEMBERSHIP)");
    //     return true;
    // }
    return false;
}

void Beacon::print(const struct sockaddr_in& sa){
    char s[256];
    inet_ntop(AF_INET, &(sa.sin_addr), s, 256);
    printf("=> Socket Address %s:%d\n", s, ntohs(sa.sin_port));
}

bool Beacon::send(string message){
    int flags = 0;  // default
    // int flags = MSG_DONTROUTE; // don't send over router
    int nbytes = sendto(
        fd,
        message.c_str(),
        message.size(),
        flags,
        (struct sockaddr*) &addr,  // wrong????
        sizeof(addr)
    );
    if (nbytes < 0) return true;
    return false;
}

bool Beacon::ready(int usec){
    struct timeval tv;
    fd_set readfds;
    int err;

    if (usec > 999999){
        printf("====================================\n");
        printf(" Beacon::ready usec < 1,000,000 usec\n");
        printf("====================================\n\n");
        // exit(1);
        usec = 999999;
    }

    tv.tv_sec = 0;
    tv.tv_usec = usec;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // don't care about writefds and exceptfds:
    // err = select(fd+1, &readfds, NULL, NULL, &tv);
    if (select(fd+1, &readfds, NULL, NULL, &tv) < 0)
        perror("Beacon::ready select issue");
    // if (err < 0) cout << "** select issue: " << err << endl;

    if (FD_ISSET(fd, &readfds)) return true;
    return false;
}

string Beacon::recv(){
    string ret;
    char msgbuf[MSGBUFSIZE];
    socklen_t addrlen = sizeof(addr);
    int nbytes = recvfrom(
        fd,
        msgbuf,
        MSGBUFSIZE,
        0,
        (struct sockaddr *) &addr,
        &addrlen
    );
    if (nbytes > 0) {
        // printf("Beacon::recv %d bytes\n", nbytes);
        // msgbuf[nbytes] = '\0';
        // ret = msgbuf;
        ret.assign(msgbuf, nbytes);
        printf("Beacon::recv %d bytes %s\n", nbytes, ret.c_str());
    }
    else if (nbytes < 0) {
        perror("Beacon::recv");
    }
    else printf("Beacon::recv empty\n");
    return ret;
}

//////////////////////////////////////////////////////////////////////////////

BeaconServer::BeaconServer(): Beacon(){}

bool BeaconServer::init(string group, int port){
    if (Beacon::initSocket(group, port, true)) {
        perror("BeaconServer::init()");
        return true;
    }

    // int yes = 1;
    // if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0){
    //    perror("Reusing ADDR failed");
    //    return true;
    // }
    // else printf("BeaconServer::init SO_REUSEADDR\n");

    // bind to all interfaces to receive address
    struct sockaddr_in aaddr;
    memset(&aaddr, 0, sizeof(aaddr));
    aaddr.sin_family = AF_INET;
    // aaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    aaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    aaddr.sin_port = htons(port);
    if (::bind(fd, (struct sockaddr*) &aaddr, sizeof(aaddr)) < 0) {
        perror("BeaconServer::listen() --> bind");
        print(aaddr);
        return true;
    }
    else {
        printf("-->> BeaconServer::listen() success bind\n");
        print(aaddr);
    }

    // // bind to all interfaces to receive address
    // if (::bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
    //     perror("BeaconServer::listen() --> bind");
    //     print(addr);
    //     return true;
    // }

    // use setsockopt() to request that the kernel join a multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
    // mreq.imr_interface.s_addr = inet_addr("10.0.1.37");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
        perror("BeaconServer::listen() --> setsockopt(IP_ADD_MEMBERSHIP)");
        return true;
    }

    printf("BeaconServer Beacon --------------------------------\n");
    printf(" Addr: %s:%d\n", group.c_str(), port);
    // printf(" Key: %s\n", key.c_str());
    printf("\n");
    print(addr);

    return false;
}

string BeaconServer::listen(int usec){
    // string msg;
    // while (msg.empty()){
    //     bool data = Beacon::ready(usec);  // data available to read?
    //     if (data) {
    //         msg = Beacon::recv();
    //         printf("BeaconServer::listen %s\n", msg.c_str());
    //     }
    // }
    //
    // return msg;
    return Beacon::recv();
}

string BeaconServer::listen_nb(int usec){
    // string msg;
    // bool data = Beacon::ready(usec);  // data available to read?
    // if (data) {
    //     msg = Beacon::recv();
    //     printf("BeaconServer::listen %s\n", msg.c_str());
    // }
    // return msg;

    string msg;
    if (Beacon::ready(usec)) msg = Beacon::recv();
    return msg;
}


//////////////////////////////////////////////////////////////////////////////


BeaconClient::BeaconClient(): Beacon(){}

bool BeaconClient::init(string group, int port){
    bool err = Beacon::initSocket(group, port, false);
    if(err){
        perror("BeaconClient::init FAILED");
        return err;
    }

    printf("BeaconClient Beacon ---------------------------------\n");
    printf(" Addr: %s:%d\n", group.c_str(), port);
    printf("\n");
    print(addr);
    return false;
}

/*
Finds the service
input:
    message: host|service
    usec: select time to wait for data in microseconds
return
    ans: response message. ans.empty() if no message found
*/
string BeaconClient::send(const string& message, int usec){
    string ans;
    if (Beacon::send(message)) perror("BeaconClient::find");
    if (Beacon::ready(usec)) {
        ans = Beacon::recv();
        printf("BeaconClient::find %s\n", ans.c_str());
        // return Beacon::recv(); // got response
    }
    else
        printf("BeaconClient::find no data\n");

    return ans;
}


string BeaconClient::find(const string& key, string& topic, int usec){
    stringstream ss;
    ss << key << sep << topic;

    return send(ss.str());
}

string BeaconClient::notify(const string& key, string& topic, const string& endpt,int usec){
    stringstream ss;
    ss << key << sep << topic << sep << endpt;

    return send(ss.str());
}
