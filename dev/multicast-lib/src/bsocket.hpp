#pragma once

#include <ssocket.hpp>
#include <string>


/*
 * Uses the broadcast address to transmit a query response pattern. Since
 * it uses broadcast, the sender will receive a copy of the message and must
 * ignore it.
 */
class BCSocket: public SSocket {
public:
    BCSocket(int port): port(port){
        sockopt(SOL_SOCKET, SO_BROADCAST, 1);
        bc_addr = make(port, INADDR_BROADCAST);
    }

    // void set_broadcast(int port){
    //     sockopt(SOL_SOCKET, SO_BROADCAST, 1);
    //     bc_addr = make(port, INADDR_BROADCAST);
    // }

    inline
    bool cast(const std::string& msg){
        return send(msg, bc_addr);
    }

    inline
    void bind(){
        SSocket::bind(port, INADDR_ANY);
    }

    void info(){
        printf("Broadcast Socket =====================================\n");
        printf("  fd: %d\n", sock);
    }

protected:
    int port;
    struct sockaddr_in bc_addr;
};
