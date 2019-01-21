// C
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include <unistd.h> // for sleep()
#include <stdio.h>
#include <stdlib.h>

// C++
// #include <chrono>
#include <string>
#include <iostream>

#include "mbeacon.hpp"
#include "time.hpp"


using namespace std;


Beacon::Beacon(string grp, int port): group(grp), port(port), MSGBUFSIZE(255), fd(0) {}

bool Beacon::initSocket(bool reuse){

    // create what looks like an ordinary UDP socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("Beacon::initSocket");
        return true;
    }

    // allow multiple sockets to use the same PORT number
    if (reuse){
        u_int yes = 1;
        int err = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes));
        if (err < 0){
           perror("Reusing ADDR failed");
           return true;
        }
    }

    // set up destination address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group.c_str());
    addr.sin_port = htons(port);

    return false;
}

bool Beacon::send(string message){
    int nbytes = sendto(
        fd,
        message.c_str(),
        message.size(),
        0,
        (struct sockaddr*) &addr,
        sizeof(addr)
    );
    if (nbytes < 0) return true;
    return false;
}

bool Beacon::ready(int usec){
    struct timeval tv;
    fd_set readfds;
    int err;

    tv.tv_sec = 0;
    tv.tv_usec = usec;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    // FD_SET(0, &readfds);  // standard in

    // don't care about writefds and exceptfds:
    err = select(fd+1, &readfds, NULL, NULL, &tv);
    if (err < 0) cout << "** select issue: " << err << endl;

    if (FD_ISSET(fd, &readfds)) return true;
    // else if (FD_ISSET(0, &readfds)) {
    //     cout << "key" << endl;
    //     return false;
    // }
    else return false;
}

// bool Beacon::recv(string& msg){
//     char msgbuf[MSGBUFSIZE];
//     socklen_t addrlen = sizeof(addr);
//     int nbytes = recvfrom(
//         fd,
//         msgbuf,
//         MSGBUFSIZE,
//         0,
//         (struct sockaddr *) &addr,
//         &addrlen
//     );
//     if (nbytes < 0) return true;
//
//     msgbuf[nbytes] = '\0';
//     msg = msgbuf;
//
//     return false;
// }

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
        msgbuf[nbytes] = '\0';
        ret = msgbuf;
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////////

Listener::Listener(string key, string grp, int port): Beacon(grp, port), key(key){
    // Beacon::initSocket(true);

    printf("Listener -------------------------------------\n");
    printf(" Addr: %s:%d\n", grp.c_str(), port);
    printf(" Key: %s\n", key.c_str());
}

bool Listener::init(){
    bool er = Beacon::initSocket(true);
    if (er) return er;

    // bind to all interfaces to receive address
    struct sockaddr_in aaddr;
    memset(&aaddr, 0, sizeof(aaddr));
    aaddr.sin_family = AF_INET;
    aaddr.sin_addr.s_addr = inet_addr("0.0.0.0"); // need to send response back
    aaddr.sin_port = htons(port);
    if (::bind(fd, (struct sockaddr*) &aaddr, sizeof(aaddr)) < 0) {
        perror("Listener::listen() --> bind");
        return true;
    }

    // use setsockopt() to request that the kernel join a multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    int err = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
    if (err < 0){
        perror("Listener::listen() --> setsockopt");
        return true;
    }
    return false;
}

string Listener::listen(){
    string msg;
    while (msg.empty()){
        bool data = Beacon::ready(1000);  // data available?
        if (data) msg = Beacon::recv();
    }

    return msg;
}

//////////////////////////////////////////////////////////////////////////////


Search::Search(string grp, int port): Beacon(grp, port){
    // Beacon::initSocket(false);

    printf("Search -------------------------------------\n");
    printf(" Addr: %s:%d\n", grp.c_str(), port);
    // printf(" Key: %s\n", key.c_str());
}

bool Search::init(){
    return Beacon::initSocket(false);
}

string Search::find(string message){
    string ans;
    for(int cnt = 50; cnt > 0; --cnt) {
        bool err = Beacon::send(message);
        if (err) perror("Search::find");

        bool data = Beacon::ready(1000);

        if (data){
            ans = Beacon::recv();
            // err = Beacon::recv(ans);
            // if (err) perror("Search::find -> recv");
            // else cout << ">> search got answer: " << ans << endl;
            break;
        }
        Time::msleep(100);
    }
    return ans;
}
