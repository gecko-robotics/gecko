#include <gecko/ps.hpp>
#include <gecko/core.hpp>
#include <gecko/defaults.hpp>
#include <thread>
#include <map>
#include <unistd.h>     // getpid
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <exception>

using namespace std;

///////////////////////////////////////////

/*
 Binders [6]
  [PID]   Topic                  CPU   MEM   EndPoint
  [65993] hello................. 0.0%  0.0%  tcp://10.0.1.57:65303
  [65994] hey there............. 0.0%  0.0%  tcp://10.0.1.57:65303
  [66008] ryan.................. 0.1%  0.0%  uds:///var/run/ryan-0
*/

using namespace gecko;

BeaconCoreServer::BeaconCoreServer(const string& key, int ttl, int delay):
    pid(getpid()), delay(delay) {

    HostInfo hi;

    if (key.size() > 0) this->key = key;
    else this->key = hi.hostname;

    host = hi.address;

    datum = time_date();
}

BeaconCoreServer::~BeaconCoreServer(){
    prnt.join();
}

void BeaconCoreServer::stop(){
    ok = false;
}

string BeaconCoreServer::handle_bind(ascii_t& data){
    // PublishTopic [4]: {key,topic,pid,endpt}
    string msg;

    // if(data.size() == 4){
        string topic = data[1];
        string pid = data[2];
        string endpt = data[3];

        if (endpt == "ok"){
            // this is an echo
            cout << "** crap echo: " << endl;
            return msg;
        }
        // {key,topic,pid,endpt/fail,ok/fail}
        // services.push(topic, endpt);
        // bind.push(topic, pid);
        // data.push_back("ok");

        services.pushbind(topic, pid, endpt);

        data[3] = "ok";

        printf(">> BIND[%s]: %s: %s\n", pid.c_str(), topic.c_str(), endpt.c_str());

        Ascii a;
        msg = a.pack(data);
        // cout << "\nbind send: " << msg << "\n" << endl;
        // ss.send(msg);
    // }

    return msg;
}

string BeaconCoreServer::handle_conn(ascii_t& data){
    // FindTopic [3]: {key,topic,pid}
    string msg;
    string topic = data[1];
    string pid = data[2];
    try {
        string endpt = services.get(topic);
        // conn.push(topic, pid);
        // services.push(topic, endpt);
        services.pushconn(topic, pid, endpt);

        // {key,topic,pid,endpt/fail,ok/fail}
        printf(">> CONN[%s]: %s: %s\n", pid.c_str(), topic.c_str(), endpt.c_str());
        // data.push_back(endpt);
        data[2] = endpt;
        data.push_back("ok");

        Ascii a;
        msg = a.pack(data);
        // cout << "\nconn send: " << msg << "\n" << endl;
        // ss.send(msg);
        // return msg;
    }
    catch (InvalidKey e){
        printf("** Invalid Topic: %s **\n", topic.c_str());
        // data.push_back("fail");

        // Ascii a;
        // string msg = a.pack(data);
        // ss.send(msg);
    }
    return msg;
}


void BeaconCoreServer::listen(bool print){
    // setup multicast
    SSocket ss;
    ss.init(mc_addr, mc_port, 1, true);

    // setup printing loop in another thread
    if (print)
        prnt = thread(&BeaconCoreServer::printLoop, this);


    Ascii a;
    while(ok){
        // string ans = ss.recv_nb();
        // MsgAddr ma = ss.recv2();
        // string ans = ma.msg;
        string ans;
        struct sockaddr_in addr;

        tie(ans, addr) = ss.recv();

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);

            // cout << "Msg: ";
            // for (const auto& s: t) cout << s << " ";
            // cout << endl;
            string msg;

            if (t.size() == 3) msg = handle_conn(t);
            else if (t.size() == 4) msg = handle_bind(t);
            ss.send(msg, addr);
        }
        // else cout << "** nothing **" << endl;
    }
    // prnt.join();
}

void BeaconCoreServer::printLoop(){
    // cout << "printLoop()" << endl;
    while(ok){
        print();
        sleep(delay);
    }
}

void BeaconCoreServer::print(){
    PS ps;
    ps.get(to_string(pid));

    printf("========================================\n");
    printf(" Geckocore [%d]\n", pid);
    printf("-------------\n");
    printf(" Start: %s\n", datum.c_str());
    printf(" Key: %s\n", key.c_str());
    printf(" Host IP: %s\n", host.c_str());
    printf(" Listening on: %s:%d\n", mc_addr.c_str(), mc_port);
    printf(" CPU: %s   Memory: %s\n", ps.cpu.c_str(), ps.mem.c_str());
    printf("-------------\n");
    services.print();
}
