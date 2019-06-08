
#include "gecko/log.hpp"
#include "gecko/signals.hpp"
#include "gecko/geckocpp.hpp"
#include <thread>
#include <mutex>
#include <pwd.h>        // geteuid,getpwuid
#include <sys/types.h>  // umask
#include <sys/stat.h>   // umask
#include <unistd.h>     // getpid
#include <iostream>


using namespace std;
// using std::mutex;

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

namespace gecko {
    static bool initialized = false;
    mutex g_mutex;
    static string host_name;
    static string host_addr;
    static string mc_addr = {"224.3.29.110"};
    static int mc_port = 11311;
    // Time time;
    Logger logger;
    static SigCapture sig; // this should be in init ... not global activation

//
// template<typename T, typename EP>
// T* binder(string key, string topic, string path){


template<typename T>
T* binder(string key, string topic, string path, string(*EP)(const string&)){

    // string addr = EP(path).endpoint;
    string addr = EP(path);
    // if (file.empty()) addr = zmqTCP(host_addr);  // bind to next available port
    // else addr = zmqUDS(file);
    // string addr = zmqTCP(host_addr);  // bind to next available port
    T *p = new T();
    p->bind(addr);
    int retry = 5;

    SSocket ss;
    ss.init(mc_addr, mc_port);

    Ascii a;
    pid_t pid = getpid();
    ascii_t tmp = {key,topic,to_string(pid),p->endpoint};
    string msg = a.pack(tmp);

    for (int i=0; i<retry; i++){
        ss.send(msg);
        printf("binder send\n");
        string ans = ss.recv_nb(900);

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            if(t.size() == 4){
                if (t.back() == "ok") {
                    string topic = t[1];
                    printf(">> BINDER[%s]: %s\n",
                        topic.c_str(),
                        t.back().c_str());
                    return p;
                }
                // else cout << "** pub t.back() " << t.back() << endl;
            }
        }
    }
    delete p;
    return nullptr;
}

template<typename T>
T* connecter(string key, string topic){
    SSocket ss;
    ss.init(mc_addr, mc_port);
    int retry = 5;

    Ascii a;
    pid_t pid = getpid();
    ascii_t tmp = {key,topic, to_string(pid)};
    string msg = a.pack(tmp);

    for (int i=0; i<retry; i++){
        ss.send(msg);
        printf("connect send\n");
        string ans = ss.recv();
        // cout << "sub ans " << ans << ' ' << ans.size() << endl;

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            if(t.size() == 3){
                // cout << "t.back() " << t.back() << endl;
                if (t[0] == key && t[1] == topic) {
                    string endpt = t[2];
                    printf(">> CONNECTOR[%s]: %s\n",topic.c_str(), endpt.c_str());

                    T *p = new T();
                    p->connect(endpt);
                    return p;
                }
                // else cout << "** invalid ans " << ans << endl;
            }
        }
    }

    return nullptr;
}

}


bool gecko::ok(){
    return sig.ok;
}

void gecko::shutdown(){
    sig.ok = false;
}

void gecko::init(string mc, int port){
    lock_guard<mutex> guard(g_mutex);
    if (initialized) return;

    sig.on();

    mc_addr = mc;
    mc_port = port;

    HostInfo h = HostInfo();
    host_addr = h.address;
    host_name = h.hostname;

    initialized = true;
    printf("gecko --------------------------\n");
    printf(" initialized: %s\n", initialized ? "true" : "false");
    printf(" %s [%s]\n", host_name.c_str(), host_addr.c_str());
    printf(" multicast: %s:%d\n", mc_addr.c_str(), mc_port);
    // printf(" key: %s\n", key.c_str());
    printf("\n");
}

Subscriber* gecko::subBindTCP(string key, string topic){
    return binder<Subscriber>(key, topic, host_addr, zmqTCP);
}

Subscriber* gecko::subBindUDS(string key, string topic, string file){
    return binder<Subscriber>(key, topic, file, zmqUDS);
}

Subscriber* gecko::subConnectTCP(string key, string topic){
    return connecter<Subscriber>(key, topic);
}

Subscriber* gecko::subConnectUDS(string key, string topic){
    return connecter<Subscriber>(key, topic);
}

Publisher* gecko::pubBindTCP(string key, string topic){
    return binder<Publisher>(key, topic, host_addr, zmqTCP);
}

Publisher* gecko::pubBindUDS(string key, string topic, string file){
    return binder<Publisher>(key, topic, file, zmqUDS);
}

Publisher* gecko::pubConnectTCP(string key, string topic){
    return connecter<Publisher>(key, topic);
}

Publisher* gecko::pubConnectUDS(string key, string topic){
    return connecter<Publisher>(key, topic);
}

void gecko::log(int level, const std::string& s){
    switch(level){
    case INFO:
        logger.loginfo(s);
        break;
    case WARN:
        logger.logwarn(s);
        break;
    case ERROR:
        logger.logerror(s);
        break;
    case DEBUG:
        logger.logdebug(s);
        break;
    }
}
