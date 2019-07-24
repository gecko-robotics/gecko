#include <iostream>
#include <string>
#include <sstream>      // std::istringstream
#include <vector>
#include <map>
#include <thread>
#include "mbeacon.hpp"
#include <unistd.h>  // getopt

using namespace std;


string ip = "239.255.255.250";
int port = 12345;

void bob(const string& topic){
    BeaconClient b;
    bool err = b.init(ip,port);
    if(err){
        printf("Search error\n");
        return;
    }

    stringstream ss;
    ss << "dalek|" << topic;
    // string s = "dalek|imu";
    // string s = "dalek|google";
    string ans;
    int cnt = 0;
    while (ans.empty()){
        cnt += 1;
        ans = b.send(ss.str());

        if (!ans.empty()){
            // double check
            vector<string> toks;
            Parser p;
            p.parse(ans, toks);
            assert(toks[0] == "dalek");
            assert(toks[1] == topic);

            printf("\n>>>>>>>>>>>>><<<<<<<<<<<<<<\n");
            printf(">> find %s[%d]: %s\n", topic.c_str(), ++cnt, ans.c_str());
            printf(">>>>>>>>>>>>><<<<<<<<<<<<<<\n\n");
            return;
        }
        else
            printf("\nCrap crackers ... couldn't find anything\n");
        sleep(1);
    }
}

void tom(void){
    // publisher database
    map<string,string> db = {
        {"time","tcp://1.2.3.4:9000"},
        {"imu","tcp://4.5.6.7:8000"},
        {"mars","tcp://14.15.16.17:18000"}
    };
    string key = "dalek";

    BeaconServer b;
    bool err = b.init(ip,port);
    if(err){
        printf("Listener error\n");
        return;
    }

    Parser p;

    //  in: key|topic
    // out: key|topic|tcp://hostname:port
    int cnt = 0;
    while(true){
        string s = b.listen();
        // cout << "** listen: "<< s << endl;
        vector<string> toks;
        p.parse(s, toks);

        cnt += 1;

        for (const string& s: toks) printf("toks: %s\n", s.c_str());

        if (toks[0] == key) {
            stringstream ans;
            auto search = db.find(toks[1]);
            if(search != db.end()){
                ans << key << "|" << toks[1] << "|" << search->second;
                b.send(ans.str());
                printf("** [%d] sending reply %s -> %s\n",
                    cnt,
                    search->first.c_str(),
                    search->second.c_str());
                printf("** %s\n", ans.str().c_str());
            }
            else
                printf("## valid key, but no topic supported: %s\n", toks[1].c_str());
            // break;
        }
    }
}


int main(){
    vector<thread*> procs;
    vector<string> topics = {"imu", "time", "mars"};
    for (int i=0; i<1; i++) {
        thread *p = new thread(bob, topics[i%3]);
        p->detach();
        procs.push_back(p);
    }
    thread b(tom); b.detach();

    while(true){}

    return 0;
}
