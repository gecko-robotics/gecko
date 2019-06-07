#include <gecko/core.hpp>
#include <string>
#include <map>
#include <unistd.h>     // getpid
#include <stdlib.h>
// #include <tuple>s
// #include <map>
#include <iostream>
#include <vector>
#include <exception>

using namespace std;

///////////////////////////////////////////

// FIXME: move else where
static string mc_addr = {"224.3.29.110"};
static int mc_port = 11311;

// FindTopic [3]: {key,topic,pid}
// ReturnTopic [5]: {key,topic,pid,endpt/fail,ok/fail}
// PublishTopic [4]: {key,topic,pid,endpt}

BeaconCoreServer::BeaconCoreServer(const string& key, int ttl):
    exit(false), pid(getpid()) {
}

void BeaconCoreServer::start(){
    // start thread
}

void BeaconCoreServer::stop(){
    exit = true;
}

void BeaconCoreServer::handle_bind(std::vector<std::string>& data){
    // PublishTopic [4]: {key,topic,pid,endpt}
    if(data.size() == 4){
        string topic = data[1];
        string pid = data[2];
        string endpt = data[3];
        // {key,topic,pid,endpt/fail,ok/fail}
        services.push(topic, endpt);
        bind.push(topic, pid);
        data.push_back("ok");

        printf(">> BIND[%s]: %s: %s\n", pid.c_str(), topic.c_str(), endpt.c_str());
    }
}

void BeaconCoreServer::handle_conn(std::vector<std::string>& data){
    // FindTopic [3]: {key,topic,pid}
    if (data.size() == 3){
        string topic = data[1];
        string pid = data[2];
        string endpt = services.get(topic);
        conn.push(topic, pid);
        services.push(topic, endpt);

        // {key,topic,pid,endpt/fail,ok/fail}
        printf(">> CONN[%s]: %s: %s\n", pid.c_str(), topic.c_str(), endpt.c_str());
    }
}

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
