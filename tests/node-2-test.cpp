#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <unistd.h>  // usleep
#include <cstdarg>

#include "time.hpp"
#include "event.hpp"
#include "signals.hpp"

// copy header from here
// https://github.com/zeromq/cppzmq
#include "zmq.hpp"
#include "zmq_addon.hpp"

using namespace std;

void sub(bool *e){
    zmq::context_t context(1);
    const string protocol = "tcp://localhost:5555";
    zmq::socket_t sock (context, ZMQ_SUB);
    sock.connect(protocol);
    sock.setsockopt(ZMQ_SUBSCRIBE, "", 0); // topics defined by ports

    Rate r(3);

    while(*e){
        zmq::message_t msg;
        sock.recv(&msg, ZMQ_DONTWAIT); // non-blocking
        // sock.recv(&msg, 0); // blocking - won't see signal if blocking
        if(msg.size() > 0){
            string s(static_cast<char*>(msg.data()), msg.size());
            printf(">> [SUB] msg[%zu]: %s\n", s.size(), s.c_str());
        }
        else printf("** No data found\n");
        r.sleep();
    }
    sock.close();
    printf(">> sub bye\n");
}

void pub(bool *e){
    zmq::context_t context(1);
    const string protocol = "tcp://*:5555";
    zmq::socket_t sock (context, ZMQ_PUB);
    sock.bind(protocol);
    Rate r(1);


    // va_list args;
    // int count = 1;
    // va_start(args, e);
    // char* test = va_arg(args, char*);
    // printf(">>> %s\n\n", test);

    while(*e){
        zmq::message_t msg((void*)"kevin", 5);
        sock.send(msg);
        printf(">> [PUB] sent msg\n");
        r.sleep();
    }
    sock.close();
    printf(">> pub bye\n");
}

// thread class member
// https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class Node: public SigCapture {
public:
    ~Node();
    void run(void(*f)(bool*));
    static void wait(){
        while(ok) {sleep(1);}
    }
    std::thread::id getId(){return the_thread.get_id();}
protected:
    std::thread the_thread;
};

Node::~Node(){
    the_thread.join();
    printf(">> Node bye ...\n");
}

void Node::run(void(*f)(bool*)){
    the_thread = thread(f, &ok);
}





int main(void){
    Node s; s.run(sub);
    Node p; p.run(pub);

    Node *n = nullptr;
    for (int i=5;i;i--) {
        n = new Node();
        n->run(sub);
    }

    Node::wait();

    return 0;
}
