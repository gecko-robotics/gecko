#pragma once

#include <thread>
#include <mutex>
#include <string>
#include <sys/types.h>  // pid (type int)
#include <unistd.h>       // sleep
#include "signals.hpp"    // SigCapture
// #include "directory.hpp"  // db
#include "transport.hpp"  // pub/sub
#include "network.hpp"    // hostinfo
#include "helpers.hpp"    // zmqtTCP
#include "ascii.hpp"
#include "msocket.hpp"


class Process {
public:
    Process(): sid(0) {}
    void go_daemon();
    void savepid(const std::string& fname);
    pid_t sid;
};

// thread class member
// https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class Node: public SigCapture {
public:
    ~Node();
    void run(void(*f)(bool*));

    std::thread::id getId();

    // static functions ---------------------------------
    static void init(std::string mc={"224.3.29.110"}, int port=11311);
    static Publisher* advertise(std::string key, std::string topic, int retry=5);
    static Subscriber* subscribe(std::string key, std::string topic, int retry=5);
    static void wait();
    static void wait(uint16_t sec);

protected:
    static bool initialized;
    static std::string host_name;
    static std::string host_addr;
    static std::string mc_addr;
    static int mc_port;

    std::thread the_thread;
    static std::mutex g_mutex;
};
