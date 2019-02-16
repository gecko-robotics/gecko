#pragma once

#include <thread>
// #include <mutex>
#include <string>
// #include <sys/types.h>  // pid (type int)
// #include <unistd.h>       // getpid
#include "signals.hpp"    // SigCapture
// #include "transport.hpp"  // pub/sub
// #include "network.hpp"    // hostinfo
// #include "helpers.hpp"    // zmqtTCP
// #include "ascii.hpp"
// #include "msocket.hpp"
// #include "time.hpp"


class Process {
public:
    Process(): sid(0) {}
    void go_daemon();
    void savepid(const std::string& fname);
    pid_t sid;
};

// thread class member
// https://rafalcieslak.wordpress.com/2014/05/16/c11-
//       stdthreads-managed-by-a-designated-class/
class Node: public SigCapture {
public:
    ~Node();
    void run(void(*f)(void*), void* p=nullptr);

    std::thread::id getId();
    static void wait();
    static void wait(uint16_t sec);

protected:
    std::thread the_thread;
};
