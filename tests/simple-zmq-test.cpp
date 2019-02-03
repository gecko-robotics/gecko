#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <unistd.h>  // usleep

#include "time.hpp"
#include "event.hpp"
#include "signals.hpp"

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"
#include "zmq_addon.hpp"

using namespace std;

void sub(Event *e){
    zmq::context_t context(1);
    const string protocol = "tcp://localhost:5555";
    zmq::socket_t sock (context, ZMQ_SUB);
    sock.connect(protocol);
    sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    Rate r(10);

    while(e->is_set()){
        zmq::message_t msg;
        sock.recv(&msg, ZMQ_DONTWAIT); // non-blocking
        // sock.recv(&msg, 0); // blocking
        if(msg.size() > 0){
            string s(static_cast<char*>(msg.data()), msg.size());
            printf(">> [SUB] msg[%zu]: %s\n", s.size(), s.c_str());
        }
        else printf("** No data found\n");
        // usleep(100000);
        r.sleep();
    }
    sock.close();
    printf(">> sub bye\n");
}

void pub(Event *e){
    zmq::context_t context(1);
    const string protocol = "tcp://*:5555";
    zmq::socket_t sock (context, ZMQ_PUB);
    sock.bind(protocol);
    Rate r(1);

    while(e->is_set()){
        zmq::message_t msg((void*)"kevin", 5);
        sock.send(msg);
        printf(">> [PUB] sent msg\n");
        r.sleep();
    }
    sock.close();
    printf(">> pub bye\n");
}

////////////////////////////////////////////////////////////////
// multipart
void msub(void){
    zmq::context_t context(1);
    const string protocol = "tcp://localhost:5556";
    zmq::socket_t sock (context, ZMQ_SUB);
    sock.connect(protocol);
    string topic = "bob";
    sock.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    // sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);  // get everything

    zmq::multipart_t multi;

    while(true){
        // zmq::message_t msg;
        // sock.recv(&msg, ZMQ_DONTWAIT); // non-blocking
        // bool ok = multi.recv(sock, ZMQ_DONTWAIT);
        bool ok = multi.recv(sock);
        if (ok) {
            printf(">> [MSUB] msg size: %zu\n", multi.size());

            // while(multi.size() > 0)
            if(multi.size() == 2){
                zmq::message_t t = multi.pop();
                string msg(static_cast<char*>(t.data()), t.size());
                zmq::message_t m = multi.pop();
                string topic(static_cast<char*>(m.data()), m.size());
                cout <<">> [MSUB] "<< topic << ": " << msg << endl;
            }
            else printf("** [MSUB] 2 crap crackers, only saw %zu msgs\n", multi.size());
        }
        else printf("** [MSUB] crap crackers\n");

        usleep(100000);
    }
    sock.close();
}

void mpub(void){
    zmq::context_t context(1);
    const string protocol = "tcp://*:5556";
    zmq::socket_t sock (context, ZMQ_PUB);
    sock.bind(protocol);

    zmq::multipart_t multi;

    while(true){
        multi.push(zmq::message_t((void*)"kevin", 5));
        multi.push(zmq::message_t((void*)"bob", 3));
        bool ok = multi.send(sock);
        if (ok) printf(">> [MPUB] sent msg\n");
        else printf("** [MPUB] crap crackers!!\n");
        sleep(1);
    }
    sock.close();
}

int main(void){
    Event e;
    e.set();  // flag == true

    SigCapture sig;

    thread s(sub, &e); s.detach();
    thread p(pub, &e); p.detach();
    // thread s2(msub); s2.detach();
    // thread p2(mpub); p2.detach();

    while(sig.ok){sleep(1);}
    e.clear();
    sleep(2);

    return 0;
}
