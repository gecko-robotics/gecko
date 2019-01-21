#include <iostream>
#include <string>
#include <sstream>      // std::istringstream
#include <vector>
#include <map>
#include <thread>
// #include <arpa/inet.h>
// #include "gecko.hpp"
// #include "msgs.hpp"
#include "mbeacon.hpp"

using namespace std;

// typedef struct {
//     string topic;
//     string host;
//     string endpoint;
//     int32_t pid;
//     MSGPACK_DEFINE(topic,endpoint,pid);
// } reqrep_t;


// class Parser {
// public:
//     Parser(char c='|'): sep(c) {}
//     bool parse(const std::string& str, std::vector<std::string>& toks){
//         stringstream ss(str);
//         string token;
//         try{
//             while(getline(ss,token,sep)) toks.push_back(token);
//         }
//         catch(exception& e){
//             cout << e.what() << endl;
//             return false;
//         }
//         return true;
//     }
//     const char sep;
// };

string ip = "239.255.255.250";
int port = 11311;

void bob(void){
    Search b;
    bool err = b.init(ip,port);
    if(err){
        printf("Search error\n");
        return;
    }

    string s = "dalek|imu";
    // string s = "dalek|google";
    string ans = b.find(s);

    if (!ans.empty()){
        printf(">> find: %s\n", ans.c_str());
    }
    else
        printf("\nCrap crackers ... couldn't find anything\n");
}

void tom(void){
    // publisher database
    map<string,string> db = {
        {"time","tcp://1.2.3.4:9000"},
        {"imu","tcp://4.5.6.7:8000"}
    };
    string topic = "imu";
    string key = "dalek";

    Listener b;
    bool err = b.init(ip,port);
    if(err){
        printf("Listener error\n");
        return;
    }

    Parser p;

    //  in: key|topic
    // out: key|topic|tcp://hostname:port
    while(true){
        string s = b.listen();
        cout << ">> listen: "<< s << endl;
        vector<string> toks;
        p.parse(s, toks);

        if (toks[0] == key) {
            stringstream ans;
            auto search = db.find(toks[1]);
            if(search != db.end()){
                ans << key << "|" << topic << "|" << search->second;
                b.send(ans.str());
            }
            else
                printf("valid key, but no topic supported\n");
            break;
        }
    }
}


int main(){
    thread a(bob);
    thread b(tom);

    a.join();
    b.join();

    return 0;
}
