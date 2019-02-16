#pragma once

#include <thread>  // sleep
// #include <chrono>  // time
#include <mutex>
#include <string>
#include <sys/types.h>  // pid (type int)
#include <unistd.h>       // getpid
#include "signals.hpp"    // SigCapture
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

    [[deprecated("Use *bind* or *connect* functions")]]
    Publisher* advertise(std::string key, std::string topic, bool bind=true);

    [[deprecated("Use *bind* or *connect* functions")]]
    Subscriber* subscribe(std::string key, std::string topic, bool bind=false);

    void wait();
    // void wait(uint16_t sec);
    bool ok();
    void shutdown();

    Subscriber* subBindTCP(std::string key, std::string topic);
    Subscriber* subBindUDS(std::string key, std::string topic, std::string file);
    Subscriber* subConnectTCP(std::string key, std::string topic);
    Subscriber* subConnectUDS(std::string key, std::string topic, std::string file);

    Publisher* pubBindTCP(std::string key, std::string topic);
    Publisher* pubBindUDS(std::string key, std::string topic, std::string file);
    Publisher* pubConnectTCP(std::string key, std::string topic);
    Publisher* pubConnectUDS(std::string key, std::string topic, std::string file);

    enum LogLevel {INFO, WARN, ERROR, DEBUG};

    void log(int level, const std::string& s);

    // double Time::get_time(){
    //     return now() - epic;
    // }

    // double Time::from_time(double datumn){
    //     return now() - datumn;
    // }
    //
    // inline void msleep(int msec){ std::this_thread::sleep_for(std::chrono::milliseconds(msec)); }
    // inline void usleep(int usec){ std::this_thread::sleep_for(std::chrono::microseconds(usec)); }
    // inline void  sleep(int  sec){ std::this_thread::sleep_for(std::chrono::seconds(sec)); }

};
