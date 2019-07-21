#include <iostream>
#include <msock.hpp>
// #include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "zmq.hpp"

using namespace std;

int main()
{
    SimpleSocket fdSocket;
    fdSocket.set_broadcast(PING_PORT_NUMBER, INADDR_BROADCAST);

    // Broadcast 5 beacon messages
    for (int i = 0; i < 5; i++)
    {
        fdSocket.broadcast(">");

        std::this_thread::sleep_for(std::chrono::milliseconds(PING_INTERVAL));
    }

    return 0;
}
