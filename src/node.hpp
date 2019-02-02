#pragma once

// #include <thread>
#include <string>
#include <unistd.h>
#include "signals.hpp"

namespace gecko {


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

// what is the value of this?
class Node {
public:
    Node();
};

class ThreadedNode: public Node, public Threaded {};

};
