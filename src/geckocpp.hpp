#pragma once

#include <thread>
#include <mutex>
#include <string>
#include <sys/types.h>  // pid (type int)
#include <unistd.h>       // getpid
#include "signals.hpp"    // SigCapture
// #include "directory.hpp"  // db
#include "transport.hpp"  // pub/sub
#include "network.hpp"    // hostinfo
#include "helpers.hpp"    // zmqtTCP
#include "ascii.hpp"
#include "msocket.hpp"
#include "time.hpp"


// thread class member
// https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
namespace gecko {

    // functions ---------------------------------
    void init(std::string mc={"224.3.29.110"}, int port=11311);
    Publisher* advertise(std::string key, std::string topic, int retry=5);
    Subscriber* subscribe(std::string key, std::string topic, int retry=5);
    // void wait();
    // void wait(uint16_t sec);
    bool ok();

    enum LogLevel {INFO, WARN, ERROR, DEBUG};

    void log(int level, const std::string& s);

};
