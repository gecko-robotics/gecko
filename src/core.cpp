#include <gecko/core.hpp>
#include <thread>
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

BeaconCoreServer::BeaconCoreServer(const string& key, int ttl, int delay):
    exit(false), pid(getpid()), delay(delay) {
    // HostInfo hi = HostInfo();
    HostInfo hi;

    if (key.size() > 0) this->key = key;
    else this->key = hi.hostname;

    host = hi.address;

    datum = time_date();
}

void BeaconCoreServer::start(){
    // start thread
    // thread(this->printLoop());
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

        if (endpt == "ok"){
            // this is an echo
            return;
        }
        // {key,topic,pid,endpt/fail,ok/fail}
        services.push(topic, endpt);
        bind.push(topic, pid);
        // data.push_back("ok");
        data[3] = "ok";

        printf(">> BIND[%s]: %s: %s\n", pid.c_str(), topic.c_str(), endpt.c_str());

        Ascii a;
        string msg = a.pack(data);
        cout << "\n" << msg << "\n" << endl;
        ss.send(msg);
    }
}

void BeaconCoreServer::handle_conn(std::vector<std::string>& data){
    // FindTopic [3]: {key,topic,pid}
    try {
        string topic = data[1];
        string pid = data[2];
        string endpt = services.get(topic);
        conn.push(topic, pid);
        services.push(topic, endpt);

        // {key,topic,pid,endpt/fail,ok/fail}
        printf(">> CONN[%s]: %s: %s\n", pid.c_str(), topic.c_str(), endpt.c_str());
        data.push_back(endpt);
        data.push_back("ok");

        Ascii a;
        string msg = a.pack(data);
        ss.send(msg);
    }
    catch (InvalidKey e){
        printf("** Invalid Key **\n");
        data.push_back("fail");

        Ascii a;
        string msg = a.pack(data);
        // ss.send(msg);
    }
}

void BeaconCoreServer::run(){}

void BeaconCoreServer::listen(){
    // setup multicast
    // SSocket ss;
    ss.init(mc_addr, mc_port);

    // setup printing loop in another thread
    // thread prnt(&BeaconCoreServer::printLoop, this);

    Ascii a;
    while(ok){
        string ans = ss.recv_nb();

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);

            cout << "Msg: ";
            for (const auto& s: t) cout << s << " ";
            cout << endl;

            if (t.size() == 3) handle_conn(t);
            else if (t.size() == 4) handle_bind(t);
        }
        else cout << "** nothing **" << endl;
    }
}

void BeaconCoreServer::printLoop(){
    while(ok){
        print();
        sleep(delay);
    }
}

void BeaconCoreServer::print(){
    printf("========================================\n");
    printf(" Geckocore [%d]\n", pid);
    printf("-------------\n");
    printf(" Start: %s\n", datum.c_str());
    printf(" Key: %s\n", key.c_str());
    printf(" Host IP: %s\n", host.c_str());
    printf(" Listening on: %s:%d\n", mc_addr.c_str(), mc_port);
    printf("-------------\n");
    printf("Known Services [%d]\n", services.size());
    services.print();
    printf("Binders [%d]\n", bind.size());
    bind.print();
    printf("Connections [%d]\n", conn.size());
    conn.print();
}
