#include "core.hpp"
#include <iostream>
#include <thread>
#include <string>
#include "network.hpp"
#include "transport.hpp"
#include "time.hpp"
#include "helpers.hpp"
#include "misc.hpp"
#include "directory.hpp"


using namespace std;

/////////////////////////////////////////////////////////////////////////
//
// void Directory::push(const string& key, const string& topic, const string& endpt){
//     db[key][topic] = endpt;
// }
//
// void Directory::pop(const string& key, const string& topic){
//     printf("*** FIXME ***\n");
// }
//
// string Directory::find(const string& key, const string& topic){
//     string ret;
//
//     auto dir = db.find(key);
//     if(dir != db.end()){
//         auto row = dir->second.find(topic);
//         if(row != dir->second.end()) ret = row->second;
//     }
//
//     return ret; // empty if not found
// }
//
// void Directory::print() const {
//     printf("====================================\n");
//     printf(" Directory keys: %d\n", numberKeys());
//     for (auto const& [key,dir]: db){
//         printf("[%s: %d]------------------\n", key.c_str(), numberTopics(key));
//         for (auto [topic,endpt]: dir){
//             printf(" %s => %s\n", topic.c_str(), endpt.c_str());
//         }
//     }
//     printf("-----------------------\n");
// }
//
// int Directory::numberKeys() const {
//     return db.size();
// }
//
// int Directory::numberTopics(const string& key) const {
//     auto dir = db.find(key);
//     if(dir != db.end()) return dir->second.size();
//     return -1; // key wasn't found
// }

////////////////////////////////////////////////////////////////////////////////
// std::vector<std::string> split(const std::string& s, char delimiter){
//     std::vector<std::string> tokens;
//     std::string token;
//     std::istringstream tokenStream(s);
//     while (std::getline(tokenStream, token, delimiter))
//     {
//       tokens.push_back(token);
//     }
//     return tokens;
// }
////////////////////////////////////////////////////////////////////////////////

Core::Core(string grp, int port): ok(true){
    HostInfo h = HostInfo();
    init(grp, port, h.hostname);
}

Core::Core(string grp, int port, string key): ok(true){
    init(grp, port, key);
}

void Core::init(string grp, int port, string key){
    this->key = key;
    this->group = grp;
    this->port = port;

    HostInfo h = HostInfo();

    printf("Core ---------------------\n");
    printf(" %s [%s]\n", h.hostname.c_str(), h.address.c_str());
    printf(" multicast on %s:%d\n", grp.c_str(), port);
    printf(" key: %s\n", key.c_str());
    printf(" addr: %p  sizeof: %lu\n", this, sizeof(*this));
    printf("\n");
}

Core::~Core(){
    ok = false;
}

void Core::run(int hertz){
    thread request(&Core::requestLoop, this);

    Rate rate(hertz);
    while(ok){
        // cout << "." << flush;
        // directory.print();
        rate.sleep();
    }
    cout << "bye" << endl;

    request.join();
}

void Core::requestLoop(void){
    printf("<<<<<<< thread started >>>>>>>>>>>\n");
    BeaconServer beacon;
    // bool err = beacon.init("239.255.255.250", 11311);
    // bool err = beacon.init(this->group, this->port);
    if(beacon.init(this->group, this->port))
        printf("\nCrap crakers ... beacon::init() failed\n\n");

    Parser par;

    /*
    Messages
     sub req: key|topic
      answer: key|topic|tcp://hostname:port

    pub note: key|topic|tcp://hostname:port
       reply: key|topic|tcp://hostname:port|ok

        perf: pid|name|cpu|memory
    */
    while(ok){
        string req = beacon.listen_nb(10000);
        if(!req.empty()){
            printf("* core listen found: %s\n", req.c_str());
            vector<string> v;
            par.parse(req, v);

            if (v[0] == key) {
                // handle advertise message
                if (v.size() == 2){
                    printf("* core:subscribe %s:%s\n",v[0].c_str(),v[1].c_str());
                    string s = directory.find(key, v[1]);
                    if(!s.empty()){
                        stringstream ans;
                        ans << key << "|" << v[1] << "|" << s;
                        beacon.send(ans.str());
                        beacon.send(ans.str());
                    }
                    else
                        printf("* valid key, but no topic supported\n");
                    break;
                }
                else if (v.size() == 3){ // handle pub notification
                    printf("* core:publish %s:%s:%s\n",v[0].c_str(),v[1].c_str(),v[2].c_str());
                    directory.push(key, v[1], v[2]);
                    stringstream ans;
                    ans << key << "|" << v[1] << "|" << v[2] << "|"<< "ok";
                    beacon.send(ans.str());
                    beacon.send(ans.str());
                    // printf("listener sent: %s\n", ans.str().c_str());
                }
            }
        }
        // else cout << '.' << flush;
    }
}
