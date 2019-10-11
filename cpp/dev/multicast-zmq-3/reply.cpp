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

    fdSocket.bind(PING_PORT_NUMBER, INADDR_ANY);

    while (true)
    {
        // fdSocket.recv();
        fdSocket.reply();
    }
    return 0;
}
