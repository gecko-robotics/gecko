#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <netinet/in.h>  // sockaddr_in

typedef std::vector<std::string> parse_t;

/*
Messages
 sub req: key|topic
  answer: key|topic|tcp://hostname:port

pub note: key|topic|tcp://hostname:port
   reply: key|topic|tcp://hostname:port|ok

    perf: pid|name|cpu|memory
*/
class Parser {
public:
    Parser(char c='|'): sep(c) {}
    // bool parse(const std::string& str, std::vector<std::string>& toks);
    bool parse(const std::string& str, parse_t& toks);

    const char sep;
};

/*
Beacon sends/receives string messages for setting up network connections

224.0.0.0 - 239.255.255.255

TTL     Scope
----------------------------------------------------------------------
   0    Restricted to the same host. Won't be output by any interface.
   1    Restricted to the same subnet. Won't be forwarded by a router.
 <32         Restricted to the same site, organization or department.
 <64 Restricted to the same region.
<128 Restricted to the same continent.
<255 Unrestricted in scope. Global.

https://beej.us/guide/bgnet/html/multi/sockaddr_inman.html
#include <netinet/in.h>

// All pointers to socket address structures are often cast to pointers
// to this type before use in various functions and system calls:

struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};


// IPv4 AF_INET sockets:

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;          // load with inet_pton()
};


// IPv6 AF_INET6 sockets:

struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};

struct in6_addr {
    unsigned char   s6_addr[16];   // load with inet_pton()
};


// General socket address holding structure, big enough to hold either
// struct sockaddr_in or struct sockaddr_in6 data:

struct sockaddr_storage {
    sa_family_t  ss_family;     // address family

    // all this is padding, implementation specific, ignore it:
    char      __ss_pad1[_SS_PAD1SIZE];
    int64_t   __ss_align;
    char      __ss_pad2[_SS_PAD2SIZE];
};
*/
class Beacon {
public:
    Beacon();
    ~Beacon();
    bool initSocket(std::string grp, int port, bool reuse);
    // bool recv(std::string& msg);
    std::string recv();
    bool send(std::string message);
    bool ready(int usec);    // uses select to wait for messages
    void print(const struct sockaddr_in& sa);
    bool ipAddMembership(bool join);

    // std::string group;       // ip address
    // const int port;          // port number
    int fd;                  // socket file descriptor
    struct sockaddr_in addr; // need for send/recv
    const int MSGBUFSIZE;    // change to array?
    char sep;
    // Parser par;
};

class BeaconServer: public Beacon {
public:
    BeaconServer();
    bool init(std::string grp={"239.255.255.250"}, int port=1900); // binds to address
    std::string listen(int usec=10000);  // key|topic -> key|topic|endpoint
    std::string listen_nb(int usec=10000);
    // std::string listen_nb(int usec);
};

class BeaconClient: public Beacon {
public:
    BeaconClient();
    bool init(std::string grp={"239.255.255.250"}, int port=1900);
    std::string send(const std::string& message, int usec=900000);  // topic:bob
    std::string find(const std::string& key, std::string& topic, int usec=900000);
    std::string notify(const std::string& key, std::string& topic, const std::string& endpt,int usec=900000);
};
