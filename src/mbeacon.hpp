#pragma once

#include <string>
#include <netinet/in.h>  // sockaddr_in


class Beacon {
public:
    Beacon(std::string grp, int port);
    bool initSocket(bool reuse);
    // bool recv(std::string& msg);
    std::string recv();
    bool send(std::string message);
    bool ready(int usec);    // uses select to wait for messages

    std::string group;       // ip address
    const int port;          // port number
    int fd;                  // socket file descriptor
    struct sockaddr_in addr; // need for send/recv
    const int MSGBUFSIZE;    // change to array?
};

class Listener: public Beacon {
public:
    Listener(std::string key, std::string grp={"239.255.255.250"}, int port=1900);
    bool init();           // binds to address
    std::string listen();  // key|topic -> key|topic|endpoint
    std::string key;       // hostname most likely, used to filter requests
};


class Search: public Beacon {
public:
    Search(std::string grp={"239.255.255.250"}, int port=1900);
    bool init();
    std::string find(std::string message);  // topic:bob
};
