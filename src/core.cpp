#include <gecko/core.hpp>
#include <string>
#include <map>
#include <unistd.h>     // getpid
#include <stdlib.h>
#include <tuple>
// #include <map>
#include <iostream>
#include <vector>
#include <exception>

using namespace std;


// void DB::print(){
//     printf("--------------------\n");
//     for (auto const& [key, val] : db) {
//         string addr;
//         string pid;
//         tie(addr, pid) = val;
//         printf(" > %-10s %-15s %-10s \n",key.c_str(),addr.c_str(),pid.c_str());
//     }
// }
//
// tuple<string, string> DB::get(const string& topic){
//     map<string, tuple<string, string>>::iterator it;
//     it = db.find(topic);
//     if (it == db.end()) throw InvalidKey(topic);
//     return it->second;
// }
//
// void DB::pop(const string& topic){
//     db.erase(topic);
// }
//
// void DB::push(const string& topic, const string& addr, const string& pid){
//     db[topic] = tuple(addr, pid);
// }

///////////////////////////////////////////

// FIXME: move else where
static string mc_addr = {"224.3.29.110"};
static int mc_port = 11311;

BeaconCoreServer::BeaconCoreServer(const string& key, int ttl){}

void BeaconCoreServer::start(){}

void BeaconCoreServer::stop(){}

void BeaconCoreServer::handle_pub(){}

void BeaconCoreServer::handle_sub(){}

void BeaconCoreServer::run(){}

void BeaconCoreServer::listen(){
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

void BeaconCoreServer::print(){
    printf("========================================\n");
    printf(" Geckocore [%d]\n", pid);
    printf("-------------\n");
    printf(" Key: %s\n", key.c_str());
    printf(" Host IP: %s\n", host.c_str());
    printf(" Listening on: %s:%d\n", mc_addr.c_str(), mc_port);
    printf("-------------\n");
    printf("Known Services [%d]\n", 1);
    printf("Binders [%d]\n", 1);
    printf("Connections [%d]\n", 1);
}
