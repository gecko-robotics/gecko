#include <marko/ssocket.hpp>

#include <sys/types.h>  // for type definitions
#include <sys/socket.h> // for socket API calls
#include <netinet/in.h> // for address structs
#include <stdio.h>      // for printf() and fprintf()
#include <stdlib.h>     // for atoi()
#include <string.h>     // for strlen()
#include <unistd.h>     // for close()
#include <string>
#include <iostream>
#include <map>
#include <errno.h>
#include <netdb.h>

using namespace std;

inline constexpr int MAX_LEN = 1024;

//
// do not bend, ha, ha, ha ...
// crease crumple cramp
// - Neuman
//

string host_ip(){
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;

    // To retrieve hostname
    int err = gethostname(hostbuffer, sizeof(hostbuffer));
    string hostname = hostbuffer;

    // see if .local is in hostname
    if (hostname.find(".local") == string::npos) hostname += ".local";

    // To retrieve host information
    host_entry = gethostbyname(hostname.c_str());
    // if (host_entry == NULL) throw HostnameError(); // FIXME

    // To convert an Internet network address into ASCII string
    return inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
}

string print_addr(const struct sockaddr_in& addr){
    string s(inet_ntoa(addr.sin_addr));
    s += ":";
    s += to_string(ntohs(addr.sin_port));
    return s;
}

// map<int, string> debug_setsockopt = {
//     {SO_REUSEADDR,       "SO_REUSEADDR"},
//     {IP_MULTICAST_TTL,   "IP_MULTICAST_TTL"},
//     {IP_MULTICAST_LOOP,  "IP_MULTICAST_LOOP"},
//     {IP_ADD_MEMBERSHIP,  "IP_ADD_MEMBERSHIP"},
//     {IP_DROP_MEMBERSHIP, "IP_DROP_MEMBERSHIP"}
// };

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

SSocket::SSocket(){
    // create a UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        throw MulticastError("SSocket::SSocket() constructor failed");
    }
}

SSocket::~SSocket(){
    ::close(sock);
}


// INADDR_ANY - bind to all available interfaces
void SSocket::bind(int port, int addr){
    // allow multiple sockets to re-use the same address and port
    sockopt(SOL_SOCKET, SO_REUSEPORT, 1);
    sockopt(SOL_SOCKET, SO_REUSEADDR, 1);

    struct sockaddr_in aaddr = make(port, addr);
    if (::bind(sock, (struct sockaddr*) &aaddr, sizeof(aaddr)) < 0) {
        throw MulticastError("SSock::bind failed");
    }
}

void SSocket::sockopt(int level, int name, int val){
    int err = setsockopt(sock, level, name, (char*) &val, sizeof(val));
    if (err < 0){
        throw MulticastError("SSocket::setsockopt " + debug_setsockopt[name] + " failed");
    }
}

bool SSocket::ready(long msec){
    struct timeval tv;
    fd_set readfds;
    long sec = 0;

    if (msec >= 1000) {
        sec = msec/1000;
        msec %= 1000;
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

    struct sockaddr_in from_addr = {0};
    unsigned int from_len = sizeof(from_addr);
    // memset(&from_addr, 0, sizeof(from_addr));

    int recv_len = 0;

    // block waiting to receive a packet
    if ((recv_len = recvfrom(sock, recv_str, MAX_LEN, 0,
         (struct sockaddr*)&from_addr, &from_len)) < 0) {
        throw MulticastError("recvfrom() failed");
    }

    // // output received string
    // printf("=> Received bytes from %s:%d\n",
    //     inet_ntoa(from_addr.sin_addr),
    //     ntohs(from_addr.sin_port));

    // string msg;
    // if (recv_len > 0) msg = recv_str;

    // return std::move(MsgAddr(msg, from_addr));
    return MsgAddr({recv_str}, from_addr);
}

MsgAddr SSocket::recv_nb(long msec){
    if(ready(msec))
        return this->recv();

    MsgAddr r = {{""}, {0}};
    return r;
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

bool SSocket::isEcho(const struct sockaddr_in& a){
    std::string ip(inet_ntoa(a.sin_addr));
    if(ip[0] == '1' && ip[1] == '2' && ip[2] == '7') return true;
    return false;
}
