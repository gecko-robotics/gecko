/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <gecko/db.hpp>
#include <gecko/gecko.hpp>
#include <string>
#include <thread>

#include <marko/ascii.hpp>

/*
https://www.tldp.org/HOWTO/Multicast-HOWTO-2.html
Bit -->  0                           31            Address Range:
        +-+----------------------------+
        |0|       Class A Address      |       0.0.0.0 - 127.255.255.255
        +-+----------------------------+
        +-+-+--------------------------+
        |1 0|     Class B Address      |     128.0.0.0 - 191.255.255.255
        +-+-+--------------------------+
        +-+-+-+------------------------+
        |1 1 0|   Class C Address      |     192.0.0.0 - 223.255.255.255
        +-+-+-+------------------------+
        +-+-+-+-+----------------------+
        |1 1 1 0|  MULTICAST Address   |     224.0.0.0 - 239.255.255.255
        +-+-+-+-+----------------------+
        +-+-+-+-+-+--------------------+
        |1 1 1 1 0|     Reserved       |     240.0.0.0 - 247.255.255.255
 */

/****
 * This is the main geckocore server. It uses multicast to communicate with
 * other gecko nodes. Because the messages are really short, the core only
 * uses simple UTF-8 ASCII characters for simplicity. This could change in the
 * future.
 */

class BeaconCoreServer: public SigCapture {
public:
    BeaconCoreServer(const std::string& key);
    ~BeaconCoreServer();

    void stop();  // shutdown core
    std::string handle_bind(ascii_t& data);
    std::string handle_conn(ascii_t& data);
    void listen(bool print=true);  // main loop to handle connections
    void set_broadcast(const std::string& addr, int port);
    void set_broadcast(int port);

protected:
    void printLoop();  // thread function, runs until signal
    void print();      // print once
    std::string key;   // core key
    std::string hostname;  // host name
    std::string address;   // host address
    std::string datum; // when did core start?
    int pid;           // core pid
    DBv services;      // bind[topic, (pid, endpt)]  conn[pid, (topic, endpt)]
    int delay;         // replace with Rate?
    std::thread prnt;
    int port;
};
