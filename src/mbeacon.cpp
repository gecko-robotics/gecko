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


bool Parser::parse(const std::string& str, std::vector<std::string>& toks){
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

bool Beacon::initSocket(string group, int port, bool reuse){

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

    // don't care about writefds and exceptfds:
    err = select(fd+1, &readfds, NULL, NULL, &tv);
    if (err < 0) cout << "** select issue: " << err << endl;

    if (FD_ISSET(fd, &readfds)) return true;
    return false;
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

Listener::Listener(): Beacon(){}

bool Listener::init(string group, int port){
    bool er = Beacon::initSocket(group, port, true);
    if (er) {
        perror("Listener::init()");
        return er;
    }

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

    printf("Listener Beacon --------------------------------\n");
    printf(" Addr: %s:%d\n", group.c_str(), port);
    // printf(" Key: %s\n", key.c_str());
    print("\n");

    return false;
}

string Listener::listen(int usec){
    string msg;
    while (msg.empty()){
        bool data = Beacon::ready(usec);  // data available to read?
        if (data) msg = Beacon::recv();
    }

    return msg;
}

string Listener::listen_nb(int usec){
    string msg;
    bool data = Beacon::ready(usec);  // data available to read?
    if (data) msg = Beacon::recv();
    return msg;
}


//////////////////////////////////////////////////////////////////////////////


Search::Search(): Beacon(){}

bool Search::init(string group, int port){
    bool err = Beacon::initSocket(group, port, false);
    if(err){
        perror("Search::init FAILED");
        return err;
    }

    printf("Search Beacon ---------------------------------\n");
    printf(" Addr: %s:%d\n", group.c_str(), port);
    print("\n");
    return false;
}

string Search::find(string message, int usec){
    string ans;
    int cnt = 50;
    while(cnt--) {
        if (Beacon::send(message)) perror("Search::find");
        if (Beacon::ready(usec)) return Beacon::recv();
    }
    return ans;
}
