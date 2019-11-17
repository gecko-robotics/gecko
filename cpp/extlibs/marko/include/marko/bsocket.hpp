#pragma once

#include <marko/ssocket.hpp>
#include <string>


/*
 * Uses the broadcast address to transmit a query response pattern. Since
 * it uses broadcast, the sender will receive a copy of the message and must
 * ignore it.
 */
class BCSocket: public SSocket {
public:
    BCSocket(int port);

    inline bool cast(const std::string& msg){return send(msg, bc_addr);}
    inline void bind(){SSocket::bind(port, INADDR_ANY);}

    void info();

protected:
    int port;
    struct sockaddr_in bc_addr;  // used by cast to broadcast message
};
