/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <string>
#include <map>
#include <tuple>
#include <exception>

struct InvalidKey : public std::exception {
    InvalidKey(const std::string &s): msg("Invalid Key: " + s) {}
    InvalidKey(): msg("Invalid Key") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};


using record_t = std::tuple<std::string, std::string>; // pid endpt


/*
 This is the data base for keeping track of things ... it is simple.
 Data is tracked 2 ways:

    - binders (topic, (pid, endpt)) are unique and tracked by topic. Only
    one process can bind to a topic name
    - connectors (pid, (topic, endpt)) are tracked by pid because there
    can be a many to one relationship between processes and topics.
 */
class DBv {
public:
    DBv(){}
    std::string get(const std::string& topic);
    void pushbind(const std::string& topic, const std::string& pid, const std::string& endpt);
    void pushconn(const std::string& topic, const std::string& pid, const std::string& endpt);
    // int size(){ return db.size(); }
    void print();

protected:
    void pop(std::map<std::string, record_t>& db, const std::string& key);
    void printDB(int type, std::map<std::string, record_t>& db);
    std::map<std::string, record_t> bind, conn;
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

// class DBs {
// public:
//     DBs(){}
//     std::string get(const std::string& key);
//     void push(const std::string& key, const std::string& val);
//     void pop(const std::string& key);
//     int size(){ return db.size(); }
//     void print();
//     void printPs();
//
// // protected:
//     // [topic, (addr, pid)]
//     std::map<std::string, std::string> db;
// };

// struct record {
//     record() {}
//     std::string pid, topic, endpt;
//     int type;
// };
