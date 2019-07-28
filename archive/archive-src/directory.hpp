#pragma once

#include <map>
#include <string>
#include <vector>

// key - robot or machine name
// topic/endpoint on key (robot or machine)
typedef std::map<std::string, std::map<std::string, std::string>> dir_map_t;

class Directory {
public:
    void push(const std::string& key, const std::string& topic, const std::string& endpt);  // add
    void pop(const std::string& key, const std::string& topic);  // remove
    std::string find(const std::string& key, const std::string& topic);
    void print() const;
    int numberKeys() const;
    int numberTopics(const std::string& key) const;
    // std::map<std::string, std::map<std::string, std::string>> db;
    dir_map_t db;
};
