#include "core.hpp"
#include <iostream>
#include <thread>
#include <string>
#include "network.hpp"
#include "transport.hpp"
#include "time.hpp"
#include "helpers.hpp"
#include "misc.hpp"


using namespace std;

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

Core::Core(string grp, int port){
    HostInfo h = HostInfo();
    key = h.hostname;

    printf("Core ---------------------\n");
    printf(" %s [%s]\n", h.hostname.c_str(), h.addr.c_str());
    printf(" multicast on %s:%d\n", grp.c_str(), port);
}

void Core::run(int hertz){
    thread request(&Core::requestLoop, this);

    Rate rate(hertz);
    while(ok){
        cout << "." << flush;
        rate.sleep();
    }
    cout << "bye" << endl;

    request.join();
}

void Core::requestLoop(void){
    Listener beacon;
    bool err = beacon.init("239.255.255.250", 11311);
    if(err) printf("\nCrap crakers ... beacon::init() failed\n\n");

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
        string req = beacon.listen_nb();
        if(!req.empty()){
            vector<string> v;
            par.parse(req, v);

            if (v[0] == key) {
                // handle advertise message
                if (v.size() == 2){
                    stringstream ans;
                    auto search = directory.find(v[1]);
                    if(search != directory.end()){
                        ans << key << "|" << search->first << "|" << search->second;
                        beacon.send(ans.str());
                    }
                    else
                        printf("valid key, but no topic supported\n");
                    break;
                }
                else if (v.size() == 3){ // handle pub notification
                    directory[v[1]] = v[2];
                    stringstream ans;
                    ans << key << "|" << v[1] << "|" << v[2] << "ok";
                    beacon.send(ans.str());
                    printf("listener, pub note: %s\n", ans.str().c_str());
                }
            }
        }
    }
}
