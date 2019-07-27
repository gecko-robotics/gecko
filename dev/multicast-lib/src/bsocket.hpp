#pragma once

#include <ssocket.hpp>
#include <string>

class BCSocket: public SSocket {
public:
    BCSocket(){}

    // void broadcast(const std::string& msg, int port){}
    void set_broadcast(int port, int addr){
        int val = 1;
        // int nResult = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (int*)&val, sizeof(int)); //
        sockopt(SOL_SOCKET, SO_BROADCAST, val);

        bc_addr = make(port, addr);
    }

    void broadcast(const std::string& buffer){
        // char buffer[PING_MSG_SIZE] = {0};
        // strcpy(&buffer[0], ">");
        int bytes = ::sendto(sock, buffer.c_str(), buffer.size() + 1, 0,
            (sockaddr*)&bc_addr, sizeof(struct sockaddr_in));
        if (bytes == SOCKET_ERROR)
        {
            printf("broadcast : sendto failed");
        }
    }

    void info(){}

protected:
    struct sockaddr_in bc_addr;
};
