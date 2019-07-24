#pragma once

#include <stdint.h>
#include <map>
#include <string>



typedef struct {
    std::string topic;
    double msg_per_sec;
    uint8_t type; // pub/sub
} connection_t;

typedef struct {
    int32_t pid;
    std::string name;
    double cpu;
    double memory;
    std::vector<connection_t> connections; // overkill?
} proc_t;

class Performance {
public:
    Performance(){}
    void pop(int32_t pid){}

    std::map<int, proc_t> db;
};
