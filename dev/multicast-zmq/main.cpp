#include <iostream>
#include <msock.hpp>
#include <atomic>
#include <iostream>
#include <string>
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

#define INFO_OUT(MSG)  std::cout << "[INFO]   " << " " << (MSG) << std::endl
#define ERROR_OUT(MSG) std::cerr << "[ERROR]  " << " " << (MSG) << std::endl

#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (SOCKET)(~1)
#define NO_ERROR 0

std::atomic<bool> g_threadInterupted(false);


struct sockaddr_in set_udp(int port, int addr){
    struct sockaddr_in saBroadcast = {0};
    saBroadcast.sin_family = AF_INET;
    saBroadcast.sin_port = htons(port);
    saBroadcast.sin_addr.s_addr = htonl(addr);
    // saBroadcast.sin_port = htons(PING_PORT_NUMBER);
    // saBroadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    return std::move(saBroadcast);
}

SOCKET make_udp(){
    // int nOptOffVal = 0;
    // int nOptOnVal = 1;
    // int nOptLen = sizeof(int);

    // Create UDP socket
    SOCKET fdSocket;
    fdSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //
    if (fdSocket == INVALID_SOCKET)
    {
        ERROR_OUT("broadcast : socket creation failed");
    }

    return fdSocket;
}

SOCKET bind_udp(SOCKET& fdSocket){
    int nOptOffVal = 0;
    int nOptOnVal = 1;
    int nOptLen = sizeof(int);
    // Ask operating system to let us do broadcasts from socket
    int nResult = setsockopt(fdSocket, SOL_SOCKET, SO_BROADCAST, (char *)&nOptOnVal, nOptLen); //
    if (nResult != NO_ERROR)
    {
        ERROR_OUT("broadcast : setsockopt SO_BROADCAST failed");
    }

    return fdSocket;
}


/**
http://hintjens.com/blog:32
Create a socket and use ZeroMQ to poll.
*/
void listener()
{
    int nResult = 0;

    // Create UDP socket
    // SOCKET fdSocket;
    // fdSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //
    // if (fdSocket == INVALID_SOCKET)
    // {
    //     ERROR_OUT("zmqListen : Socket creation failed");
    // }

    SOCKET fdSocket = make_udp();

    // Set up the sockaddr structure
    // struct sockaddr_in saListen = {0};
    // saListen.sin_family = AF_INET;
    // saListen.sin_port = htons(PING_PORT_NUMBER);
    // saListen.sin_addr.s_addr = htonl(INADDR_ANY);
    struct sockaddr_in saListen = set_udp(PING_PORT_NUMBER, INADDR_ANY);

    // Bind the socket
    nResult = bind(fdSocket, (sockaddr*)&saListen, sizeof(saListen));
    if (nResult != NO_ERROR)
    {
        ERROR_OUT("zmqListen : socket bind failed");
    }

    while (!g_threadInterupted)
    {
        // Poll socket for a message
        zmq::pollitem_t items[] = {NULL, fdSocket, ZMQ_POLLIN, 0};
        zmq::poll(&items[0], 1, SOCKET_POLL_TIMEOUT);

        // If we get a message, print the contents
        if (items[0].revents & ZMQ_POLLIN)
        {
            char recvBuf[PING_MSG_SIZE] = {0};

            socklen_t saSize = sizeof(struct sockaddr_in);

            size_t size = recvfrom(fdSocket, recvBuf, PING_MSG_SIZE + 1, 0, (sockaddr*)&saListen, &saSize);
            {
                std::string ip(inet_ntoa(saListen.sin_addr));
                INFO_OUT("received: " + std::string(recvBuf) + " from " + ip);
            }
        }
    }
}


/**

Main function starts a listener thread and then sends out 5 broadcast pings
*/
int main()
{
    g_threadInterupted = false;

    // Start listener in a seperate thread
    std::thread listenerThread(listener);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    {
        int nResult = 0;
        int nOptOffVal = 0;
        int nOptOnVal = 1;
        int nOptLen = sizeof(int);

        // // Create UDP socket
        // SOCKET fdSocket;
        // fdSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //
        // if (fdSocket == INVALID_SOCKET)
        // {
        //     ERROR_OUT("broadcast : socket creation failed");
        // }
        //
        // // Ask operating system to let us do broadcasts from socket
        // nResult = setsockopt(fdSocket, SOL_SOCKET, SO_BROADCAST, (char *)&nOptOnVal, nOptLen); //
        // if (nResult != NO_ERROR)
        // {
        //     ERROR_OUT("broadcast : setsockopt SO_BROADCAST failed");
        // }


        SOCKET fdSocket = make_udp();
        fdSocket = bind_udp(fdSocket);

        // // Set up the sockaddr structure
        // struct sockaddr_in saBroadcast = {0};
        // saBroadcast.sin_family = AF_INET;
        // saBroadcast.sin_port = htons(PING_PORT_NUMBER);
        // saBroadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        struct sockaddr_in saBroadcast = set_udp(PING_PORT_NUMBER, INADDR_BROADCAST);

        // Broadcast 5 beacon messages
        for (int i = 0; i < 5; i++)
        {
            char buffer[PING_MSG_SIZE] = {0};
            strcpy(&buffer[0], ">");
            int bytes = sendto(fdSocket, buffer, PING_MSG_SIZE + 1, 0, (sockaddr*)&saBroadcast, sizeof(struct sockaddr_in));
            if (bytes == SOCKET_ERROR)
            {
                ERROR_OUT("broadcast : sendto failed");
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(PING_INTERVAL));
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    g_threadInterupted = true;

    listenerThread.join();

    return 0;
}
