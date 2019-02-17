#pragma once

#include <string>
#include "transport.hpp"  // pub/sub


namespace gecko {

    // functions ---------------------------------
    void init(std::string mc={"224.3.29.110"}, int port=11311);

    // [[deprecated("Use *bind* or *connect* functions")]]
    // Publisher* advertise(std::string key, std::string topic, bool bind=true);
    //
    // [[deprecated("Use *bind* or *connect* functions")]]
    // Subscriber* subscribe(std::string key, std::string topic, bool bind=false);

    void wait();
    bool ok();
    void shutdown();

    Subscriber* subBindTCP(std::string key, std::string topic);
    Subscriber* subBindUDS(std::string key, std::string topic, std::string file);
    Subscriber* subConnectTCP(std::string key, std::string topic);
    Subscriber* subConnectUDS(std::string key, std::string topic);

    Publisher* pubBindTCP(std::string key, std::string topic);
    Publisher* pubBindUDS(std::string key, std::string topic, std::string file);
    Publisher* pubConnectTCP(std::string key, std::string topic);
    Publisher* pubConnectUDS(std::string key, std::string topic);

    enum LogLevel {INFO, WARN, ERROR, DEBUG};

    void log(int level, const std::string& s);
};
