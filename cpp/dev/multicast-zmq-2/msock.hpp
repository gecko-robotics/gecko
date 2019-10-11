#pragma once

#include <stdio.h>
#include <iostream>
#include <thread>
#include <utility>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "zmq.hpp"


#define PING_PORT_NUMBER    9999
#define PING_MSG_SIZE       2
#define PING_INTERVAL       500
#define SOCKET_POLL_TIMEOUT 3000
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (SOCKET)(~1)
#define NO_ERROR 0

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

    void broadcast(int port, int addr){
        int nOptOffVal = 0;
        int nOptOnVal = 1;
        int nOptLen = sizeof(int);
        // Ask operating system to let us do broadcasts from socket
        int nResult = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&nOptOnVal, nOptLen); //
        if (nResult != NO_ERROR)
        {
            printf("broadcast : setsockopt SO_BROADCAST failed");
        }

        saddr = create_sockaddr(port, addr);
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
            {
                std::string ip(inet_ntoa(saddr.sin_addr));
                std::cout << "received: " + std::string(recvBuf) + " from " + ip << std::endl;
            }
        }
        else
            std::cout << ">> no data" << std::endl;
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

protected:
    SOCKET sock;
    struct sockaddr_in saddr;
    char recvBuf[PING_MSG_SIZE];
};
