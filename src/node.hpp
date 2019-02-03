#pragma once

#include <thread>
#include <string>
#include <unistd.h>     // sleep
#include "signals.hpp"  // SigCapture

// namespace gecko {


class Process {
public:
    Process(): sid(0) {}
    void go_daemon();
    void savepid(const std::string& fname);
    pid_t sid;
};

/*
Spins off a thread for a given function
*/
class Threaded: protected SigCapture {
public:
    void run(void(*f)(void*));
};

// // what is the value of this?
// class Node {
// public:
//     Node();
// };

// class ThreadedNode: public Node, public Threaded {};
//
// };

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

// Node::~Node(){
//     the_thread.join();
//     printf(">> Node bye ...\n");
// }
//
// void Node::run(void(*f)(bool*)){
//     the_thread = thread(f, &ok);
// }
