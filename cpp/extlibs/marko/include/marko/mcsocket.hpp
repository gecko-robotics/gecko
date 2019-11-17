#pragma once

#include <marko/ssocket.hpp>
#include <string>

/*
 * This uses multicast to create a quesry response pattern. By default,
 */
class MCSocket: public SSocket {
public:
    MCSocket(const std::string& group, int port);
    ~MCSocket();

    inline bool cast(const std::string& msg){return send(msg, mc_addr);}
    inline void bind(){SSocket::bind(port, INADDR_ANY);}
    inline void timeToLive(int ttl){sockopt(IPPROTO_IP, IP_MULTICAST_TTL, ttl);}

    void info();

    // If you plan to have more than one process or user "listening",
    // loopback must be enabled.
    // 0-disable  1-enable
    inline
    void multicastLoop(bool val){sockopt(IPPROTO_IP, IP_MULTICAST_LOOP, val ? 1 : 0);}

protected:
    struct sockaddr_in mc_addr;
    std::string save_grp;
    int port;
};
