#pragma once

#include <stdio.h>
#include <iostream>
#include <thread>
#include <utility>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "zmq.hpp"

// czmq
// #if !defined (__WINDOWS__)
// typedef int SOCKET;
// #   define closesocket      close
// #   define INVALID_SOCKET   -1
// #   define SOCKET_ERROR     -1
// #   if !defined O_BINARY
// #       define O_BINARY      0
// #   endif
// #endif


#define PING_PORT_NUMBER    9999
#define PING_MSG_SIZE       2
#define PING_INTERVAL       500
#define SOCKET_POLL_TIMEOUT 3000
// #define SOCKET int
typedef int SOCKET;
inline constexpr int INVALID_SOCKET = -1;
inline constexpr int SOCKET_ERROR = -1;
inline constexpr int NO_ERROR = 0;
// #define INVALID_SOCKET (SOCKET)(~0)
// #define SOCKET_ERROR (SOCKET)(~1)
// #define NO_ERROR 0

static
struct sockaddr_in create_sockaddr(int port, int addr){
    struct sockaddr_in saBroadcast = {0};
    saBroadcast.sin_family = AF_INET;
    saBroadcast.sin_port = htons(port);
    saBroadcast.sin_addr.s_addr = htonl(addr);
    return std::move(saBroadcast);
}


class SimpleSocket {
public:
    SimpleSocket(): sock(0){
        this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (this->sock == INVALID_SOCKET)
            printf("broadcast : socket creation failed");
    }

    void set_broadcast(int port, int addr){
        int val = 1;
        // int nResult = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (int*)&val, sizeof(int)); //
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (int*)&val, sizeof(int)) != NO_ERROR)
        {
            printf("broadcast : setsockopt SO_BROADCAST failed");
            exit(1);
        }

        bc_addr = create_sockaddr(port, addr);
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

    void bind(int port, int addr){
        int on = 1;
        if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR,
                    (char *) &on, sizeof (on)) == SOCKET_ERROR)
                    printf("can't reuse");

        saddr = create_sockaddr(port, addr);
        int nResult = ::bind(sock, (sockaddr*)&saddr, sizeof(saddr));
        if (nResult != NO_ERROR)
        {
            printf("zmqListen : socket bind failed");
        }
    }

    bool isEcho(const struct sockaddr_in& a){
        std::string ip(inet_ntoa(saddr.sin_addr));
        if(ip[0] == '1' && ip[1] == '2' && ip[2] == '7'){
            printf("echo ...");
            return true;
        }
        return false;
    }

    void recv(){
        zmq::pollitem_t items[] = {NULL, sock, ZMQ_POLLIN, 0};
        zmq::poll(&items[0], 1, SOCKET_POLL_TIMEOUT);

        // If we get a message, print the contents
        if (items[0].revents & ZMQ_POLLIN)
        {
            char recvBuf[PING_MSG_SIZE] = {0};

            socklen_t saSize = sizeof(struct sockaddr_in);

            size_t size = recvfrom(sock, recvBuf, PING_MSG_SIZE + 1, 0,
                (sockaddr*)&saddr, &saSize);

            if (isEcho(saddr)) return;
            {
                std::string ip(inet_ntoa(saddr.sin_addr));
                std::cout << "recv: " + std::string(recvBuf) + " from " + ip << std::endl;
            }
        }
        else
            std::cout << ">> recv no data" << std::endl;
    }

    void reply(){
        zmq::pollitem_t items[] = {NULL, sock, ZMQ_POLLIN, 0};
        zmq::poll(&items[0], 1, SOCKET_POLL_TIMEOUT);

        // If we get a message, print the contents
        if (items[0].revents & ZMQ_POLLIN)
        {
            char recvBuf[PING_MSG_SIZE] = {0};

            socklen_t saSize = sizeof(struct sockaddr_in);  // why?
            struct sockaddr_in tmp; // tmp

            size_t size = recvfrom(sock, recvBuf, PING_MSG_SIZE + 1, 0,
                (sockaddr*)&tmp, &saSize);

            // handle echos ---------------------
            if (recvBuf[0] == 'h'){
                printf("echo ...\n");
                return;
            }
            if(isEcho(tmp)) return;

            std::string ip(inet_ntoa(tmp.sin_addr));
            std::cout << "reply::recv: " + std::string(recvBuf) + " from " + ip << " sending reply" << std::endl;

            this->sendto("h", tmp);
        }
        else
            std::cout << ">> reply no data" << std::endl;
    }

    void sendto(const std::string& buffer){
        // char buffer[PING_MSG_SIZE] = {0};
        // strcpy(&buffer[0], ">");
        int bytes = ::sendto(sock, buffer.c_str(), buffer.size() + 1, 0,
            (sockaddr*)&saddr, sizeof(struct sockaddr_in));
        if (bytes == SOCKET_ERROR)
        {
            printf("broadcast : sendto failed");
        }
    }

    void sendto(const std::string& buffer, struct sockaddr_in& addr){
        // char buffer[PING_MSG_SIZE] = {0};
        // strcpy(&buffer[0], ">");
        int bytes = ::sendto(sock, buffer.c_str(), buffer.size() + 1, 0,
            (sockaddr*)&saddr, sizeof(struct sockaddr_in));
        if (bytes == SOCKET_ERROR)
        {
            printf("broadcast : sendto failed");
        }
    }

protected:
    SOCKET sock;
    // SOCKET reply;  // reply message
    struct sockaddr_in saddr;
    struct sockaddr_in bc_addr;
    struct sockaddr_in lsn_addr;
    char recvBuf[PING_MSG_SIZE];
};
