/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <string>
#include <map>
#include <tuple>

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

class DBs {
public:
    DBs(){}
    std::string get(const std::string& key);
    void push(const std::string& key, const std::string& val);
    void pop(const std::string& key);
    int size(){ return db.size(); }
    void print();
    void printPs();

// protected:
    // [topic, (addr, pid)]
    std::map<std::string, std::string> db;
};
