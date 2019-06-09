/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

// #include <map>
// #include <gecko/ascii.hpp>
#include <gecko/db.hpp>
#include <gecko/gecko.hpp>
#include <string>
// #include <tuple>
// #include <vector>
// #include <exception>
// #include <unistd.h>     // getpid


class BeaconCoreServer: public SigCapture {
public:
    BeaconCoreServer(const std::string& key, int ttl=1, int delay=3);
    void stop();  // shutdown core
    std::string handle_bind(ascii_t& data);
    std::string handle_conn(ascii_t& data);
    void listen(bool print=true);  // main loop to handle connections

protected:
    void printLoop(); // thread function, runs until signal
    void print();     // print once
    std::string key;
    std::string host;
    std::string datum;
    int pid;
    DBv services; // [topic, (pid, endpt)]
    int delay;
    // SSocket ss;

};


// struct InvalidKey : public std::exception {
//     InvalidKey(const std::string &s): msg("Invalid Key: " + s) {}
//     InvalidKey(): msg("Invalid Key") {}
//     const char * what () const throw () {return msg.c_str();}
// protected:
//     std::string msg;
// };

// class DB {
// public:
//     DB(){}
//     std::tuple<std::string, std::string> get(const std::string& topic);
//     void push(const std::string& topic, const std::string& addr, const std::string& pid);
//     void pop(const std::string& topic);
//     int size(){ return db.size(); }
//     void print();
//
// // protected:
//     // [topic, (addr, pid)]
//     std::map<std::string, std::tuple<std::string, std::string>> db;
// };
