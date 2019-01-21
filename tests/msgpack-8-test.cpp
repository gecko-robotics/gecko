#include <thread>
#include <unistd.h>
#include <iostream>
#include <string>
#include <ctime>
#include <zmq.hpp>
#include <msgpack.hpp>
#include "transport.hpp"
#include "msgs.hpp"
#include "serialization.hpp"

using namespace std;

/*
for 32/64 bit OS
/usr/include/bits/typesizes.h
#define __UID_T_TYPE            __U32_TYPE
#define __GID_T_TYPE            __U32_TYPE
#define __PID_T_TYPE            __S32_TYPE
*/
typedef struct {
    string topic;
    string endpoint;
    int32_t pid;
    MSGPACK_DEFINE(topic,endpoint,pid);
} reqrep_t;

void sub()
{
    reqrep_t t = {"hi", "hello",12345};
    reqrep_t t2 = {"hi2"};
    cout << t.topic << t.endpoint << endl;
    cout << t2.topic << t2.endpoint << endl;
    // string endpt("tcp://127.0.0.1:12900");
    string endpt("ipc:///tmp/0");
    Subscriber s(endpt);
    Transport<imu_t> buffer;

    double last = 0;
    while (true)
    {
        zmq::message_t msg = s.recv();
        imu_t m = buffer.unpack(msg);
        cout << "time diff: " << m.timestamp - last << endl;
        last = m.timestamp;
    }
}

void pub()
{
    // string endpt("tcp://127.0.0.1:12900");
    string endpt("ipc:///tmp/0");
    Publisher p(endpt);

    Transport<imu_t> buffer;

    while (true)
    {
        vec_t a(1,2,3);
        imu_t b(a,a,a);  // new timestamp
        zmq::message_t msg = buffer.pack(b);

        p.pub(msg);

        std::cout << "pub" << std::endl;
        sleep(1);
        // usleep(100000);
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
