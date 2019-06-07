/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <map>
#include <gecko/gecko.hpp>
#include <string>
#include <tuple>
#include <vector>
#include <exception>
#include <unistd.h>     // getpid


struct InvalidKey : public std::exception {
    InvalidKey(const std::string &s): msg("Invalid Key: " + s) {}
    InvalidKey(): msg("Invalid Key") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

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


class BeaconCoreServer: public SigCapture {
public:
    BeaconCoreServer(const std::string& key, int ttl=1);
    void start();
    void stop();
    void handle_bind(std::vector<std::string>& data);
    void handle_conn(std::vector<std::string>& data);
    void run();
    void listen();
    void print();

protected:
    std::string key;
    std::string host;
    int pid;
    // DB bind, conn;  // [topic, (addr, pid)]
    DBs services;  // [topic, endpt]
    DBs bind, conn;
    bool exit;

};
