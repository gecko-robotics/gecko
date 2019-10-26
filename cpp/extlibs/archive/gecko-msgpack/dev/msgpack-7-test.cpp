
/*
https://github.com/jordansissel/experiments/blob/master/c/zeromq/msgpack-zeromq.c
msgpack_sbuffer* buffer = msgpack_sbuffer_new();
zmq_msg_t message;
//printf("%zd: %.*s\n", buffer->size, buffer->size, buffer->data);
zmq_msg_init_data(&message, buffer->data, buffer->size, NULL, NULL);
rc = zmq_send(socket, &message, 0);
*/

// https://gist.github.com/mashiro/5150508
#include <thread>
#include <unistd.h>
#include <iostream>
// #include <sstream>
#include <string>
#include <ctime>
#include <zmq.hpp>
#include <msgpack.hpp>
#include "transport.hpp"
#include "msgs.hpp"
#include "serialization.hpp"

using namespace std;

void sub()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_SUB);
    socket.connect("tcp://127.0.0.1:12900");

    const std::string tag = "";  // subscribe to all
    socket.setsockopt(ZMQ_SUBSCRIBE, tag.c_str(), tag.size());

    MsgPack<imu_st> buffer;

    double last = 0;

    while (true)
    {
        zmq::message_t msg;
        socket.recv(&msg);
        imu_st m = buffer.unpack(msg);
        // m.print();
        cout << "time diff: " << m.timestamp - last << endl;
        last = m.timestamp;
    }
}


void pub()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.bind("tcp://127.0.0.1:12900");

    MsgPack<imu_st> buffer;

    while (true)
    {
        vec_t a(1,2,3);
        imu_st b(a,a,a);  // new timestamp
        zmq::message_t msg = buffer.pack(b);

        socket.send(msg);

        std::cout << "pub" << std::endl;
        sleep(1);
    }
}

int main(){

    std::cout << "start" << std::endl;
    std::thread t1(sub);
    std::thread t2(pub);
    t1.join();
    t2.join();

    return 0;
}
