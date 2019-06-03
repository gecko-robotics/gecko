/********************************************\
 * The MIT License (MIT)
 * Copyright (c) 2014 Kevin Walchko
 * see LICENSE for full details
\********************************************/
#include "gecko/gecko.hpp"
// #include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <map>
#include <unistd.h>     // getpid
#include <stdlib.h>
#include <tuple>
// #include <map>
// #include <string>
#include <vector>
#include <exception>

/*
========================================
 Geckocore [65975]
-------------
 Key: local
 Host IP: 10.0.1.57
 Listening on: 224.3.29.110:11311
-------------
Known Services [6]
 * hello:........................ tcp://10.0.1.57:65303
 * hey there:.................... tcp://10.0.1.57:65304
 * ryan:......................... tcp://10.0.1.57:65310
 * mike:......................... tcp://10.0.1.57:65311
 * sammie:....................... tcp://10.0.1.57:65312
 * scott:........................ tcp://10.0.1.57:65313

Binders [6]
 [65993] hello................. cpu:  0.0%  mem:  0.0%
 [65994] hey there............. cpu:  0.0%  mem:  0.0%
 [66008] ryan.................. cpu:  0.1%  mem:  0.0%
 [66010] mike.................. cpu:  0.1%  mem:  0.0%
 [66012] sammie................ cpu:  0.1%  mem:  0.0%
 [66014] scott................. cpu:  0.1%  mem:  0.0%

Connections [8]
 [65995] hello................. cpu: 20.7%  mem:  0.0%
 [65996] hello................. cpu: 20.9%  mem:  0.0%
 [65997] hey there............. cpu: 21.0%  mem:  0.0%
 [65998] hey there............. cpu: 20.8%  mem:  0.0%
 [66011] mike.................. cpu: 19.0%  mem:  0.0%
 [66013] sammie................ cpu: 19.0%  mem:  0.0%
 [66015] scott................. cpu: 19.4%  mem:  0.0%
 [66009] ryan.................. cpu: 18.7%  mem:  0.0%
*/


// int getFreePort(void){
//     boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 0);
//     boost::asio::io_service io_service;
//     boost::asio::ip::tcp::socket socket(io_service, ep);
//     return socket.local_endpoint().port();
// }

using namespace std;

// FIXME: move else where
static string mc_addr = {"224.3.29.110"};
static int mc_port = 11311;

struct InvalidKey : public std::exception {
    InvalidKey(const std::string &s): msg("Invalid Key: " + s) {}
    InvalidKey(): msg("Invalid Key") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

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
    map<std::string, std::tuple<std::string, std::string>> db;
};

void DB::print(){
    printf("--------------------\n");
    for (auto const& [key, val] : db) {
        string addr;
        string pid;
        tie(addr, pid) = val;
        printf(" > %-10s %-15s %-10s \n",key.c_str(),addr.c_str(),pid.c_str());
    }
}

std::tuple<std::string, std::string> DB::get(const std::string& topic){
    map<string, tuple<string, string>>::iterator it;
    it = db.find(topic);
    if (it == db.end()) throw InvalidKey(topic);
    return it->second;
}

void DB::pop(const std::string& topic){
    db.erase(topic);
}

void DB::push(const std::string& topic, const std::string& addr, const std::string& pid){
    db[topic] = tuple(addr, pid);
}

class Core {
public:
    Core(std::string k): key(k), pid(getpid()) {}

    void listen(){
        printf("========================================\n");
        printf(" Geckocore [%d]\n", pid);
        printf("-------------\n");
        printf(" Key: %s\n", key.c_str());
        // printf(" Host IP: %s\n");
        printf(" Listening on: %s:%d\n",mc_addr.c_str(), mc_port);
        printf("-------------\n");

        SSocket ss;
        ss.init(mc_addr, mc_port);

        Ascii a;
        // pid_t pid = getpid();
        // ascii_t tmp = {key,topic,to_string(pid),p->endpoint};
        // string msg = a.pack(tmp);
        while(true){
            string ans = ss.recv(900);

            if(!ans.empty()){
                ascii_t t = a.unpack(ans);

                cout << "Msg: ";
                for (const auto& s: t) cout << s << " ";
                cout << endl;
            }
        }
    }

    void print(){
        printf("========================================\n");
        printf(" Geckocore [%d]\n", pid);
        printf("-------------\n");
        printf(" Key: %s\n");
        printf(" Host IP: %s\n");
        printf(" Listening on: %s\n");
        printf("-------------\n");
        printf("Known Services [%d]\n");
        printf("Binders [%d]\n");
        printf("Connections [%d]\n");
    }

protected:
    std::string key;
    int pid;
    DB db;
};

int main(){
    Core core("local");
    core.listen();

    return 0;
}
