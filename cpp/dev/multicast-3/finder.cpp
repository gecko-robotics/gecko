#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <iostream>

#include "msocket.hpp"

using namespace std;

typedef std::vector<std::string> ascii_t;

class Ascii {
public:
    ascii_t& unpack(const string& str);
    string pack(ascii_t& v, char sep='|');

protected:
    ascii_t toks;
};

ascii_t& Ascii::unpack(const string& str){
    toks.clear();
    char sep = '|';
    stringstream ss(str);
    string token;
    try{
        while(getline(ss,token,sep)) toks.push_back(token);
    }
    catch(exception& e){
        cout << e.what() << endl;
        toks.clear();
        return toks;
    }
    return toks;
}

string Ascii::pack(ascii_t& v, char sep){
    stringstream ss;
    ss << v[0];
    for (int i=1; i<v.size(); i++) ss << sep << v[i];
    return ss.str();
}

bool advertise(std::string key, std::string topic, int pid, std::string endpt, int retry=5){
    SSocket ss;
    ss.init();

    Ascii a;
    ascii_t tmp = {key,topic,to_string(pid),endpt};
    string msg = a.pack(tmp);

    for (int i=0; i<retry; i++){
        ss.send(msg);
        printf("pub send\n");
        string ans = ss.recv();

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            if(t.size() == 4){
                if (t.back() == "ok") {
                    string topic = t[1];
                    printf(">> PUB[%s]: %s\n", topic.c_str(), t.back().c_str());
                    return true;
                }
                else cout << "** pub t.back() " << t.back() << endl;
            }
        }
    }
    return false;
}

string subscribe(std::string key, std::string topic, int pid, int retry=5){
    SSocket ss;
    ss.init();

    Ascii a;
    ascii_t tmp = {key,topic,to_string(pid)};
    string msg = a.pack(tmp);

    for (int i=0; i<retry; i++){
        ss.send(msg);
        printf("sub send\n");
        string ans = ss.recv();
        cout << "sub ans " << ans << ' ' << ans.size() << endl;

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            if(t.size() == 3){
                // cout << "t.back() " << t.back() << endl;
                if (t[0] == key && t[1] == topic) {
                    string endpt = t[2];
                    printf(">> SUB[%s]: %s\n",topic.c_str(), endpt.c_str());
                    return endpt;
                }
                else cout << "** invalid ans " << ans << endl;
            }
        }
    }
    string empty;
    return empty;

}

int main(){
    // sub: key|topic|pid
    // => sub: key|topic|endpt
    // pub: key|topic|pid|endpt
    // => pub: key|topic|endpt|ok
    SSocket ss;
    ss.init("224.3.29.110", 11311);

    vector<string> topics = {"tom","bob","sally","jane"};

    for (const string& topic: topics){
        bool pub = advertise("local", topic, 12345, "tcp://test.local:8000");

        sleep(1);

        string sub = subscribe("local", topic, 2468);
        printf(">> sub: %s\n", sub.c_str());
    }

    printf("bye ... \n");

    return 0;
}
