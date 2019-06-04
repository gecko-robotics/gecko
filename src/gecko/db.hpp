#pragma once

#include <string>
#include <map>
#include <tuple>


class DB {
public:
    DB(){}
    std::tuple<std::string, std::string> get(const std::string& topic);
    void push(const std::string& topic, const std::string& addr, const std::string& pid);
    void pop(const std::string& topic);
    int size(){ return db.size(); }
    void print();

// protected:
    // [topic, (addr, pid)]
    std::map<std::string, std::tuple<std::string, std::string>> db;
};