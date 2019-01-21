#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <netinet/in.h>  // sockaddr_in

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
    bool parse(const std::string& str, std::vector<std::string>& toks);

    const char sep;
};

class Beacon {
public:
    Beacon();
    bool initSocket(std::string grp, int port, bool reuse);
    // bool recv(std::string& msg);
    std::string recv();
    bool send(std::string message);
    bool ready(int usec);    // uses select to wait for messages

    // std::string group;       // ip address
    // const int port;          // port number
    int fd;                  // socket file descriptor
    struct sockaddr_in addr; // need for send/recv
    const int MSGBUFSIZE;    // change to array?
};

class Listener: public Beacon {
public:
    Listener();
    bool init(std::string grp={"239.255.255.250"}, int port=1900); // binds to address
    std::string listen();  // key|topic -> key|topic|endpoint
    std::string listen_nb();
};


class Search: public Beacon {
public:
    Search();
    bool init(std::string grp={"239.255.255.250"}, int port=1900);
    std::string find(std::string message);  // topic:bob
};
