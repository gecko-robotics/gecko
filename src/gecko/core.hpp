/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <gecko/db.hpp>
#include <gecko/gecko.hpp>
#include <string>


/****
 * This is the main geckocore server. It uses multicast to communicate with
 * other gecko nodes. Because the messages are really short, the core only
 * uses simple UTF-8 ASCII characters for simplicity. This could change in the
 * future.
 */

class BeaconCoreServer: public SigCapture {
public:
    BeaconCoreServer(const std::string& key, int ttl=1, int delay=3);
    void stop();  // shutdown core
    std::string handle_bind(ascii_t& data);
    std::string handle_conn(ascii_t& data);
    void listen(bool print=true);  // main loop to handle connections

protected:
    void printLoop();  // thread function, runs until signal
    void print();      // print once
    std::string key;   // core key
    std::string host;  // host address
    std::string datum; // when did core start?
    int pid;           // core pid
    DBv services;      // bind[topic, (pid, endpt)]  conn[pid, (topic, endpt)]
    int delay;         // replace with Rate?
};
