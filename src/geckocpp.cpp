
#include <gecko/geckocpp.hpp>
#include "gecko/log.hpp"
#include "gecko/signals.hpp"
#include <gecko/defaults.hpp>
#include <gecko/exceptions.hpp>
#include <thread>
#include <mutex>
#include <pwd.h>        // geteuid,getpwuid
#include <sys/types.h>  // umask
#include <sys/stat.h>   // umask
#include <unistd.h>     // getpid
#include <iostream>
#include <atomic>
#include <exception>


using namespace std;
// using std::mutex;

// #define EXIT_FAILURE 1
// #define EXIT_SUCCESS 0

namespace gecko {
class Hobo: public SigCapture {
public:
    Hobo(const Hobo&)=delete;
    Hobo& operator=(const Hobo&)=delete;
    Hobo(Hobo &&)=delete;

    static Hobo& getInstance() {
        static Hobo instance;
        // static const std::unique_ptr<Hobo> instance{new Hobo{}};
        return instance;
        // return std::move(instance);
    }

    inline int* getAddress(){return (int*)this;}

    string host_name, host_addr;
    bool initialized;
    int mc_port;
    std::string mc_addr;

private:
    Hobo(): initialized(false), mc_port(GECKOCORE_PORT), mc_addr(GECKOCORE_ADDRESS){
        printf(">> Hobo constructor\n");
        on();
        initialized = true;

        HostInfo h = HostInfo();
        host_addr = h.address;
        host_name = h.hostname;
    }

    ~Hobo(){
        printf(">> Hobo exiting\n");
    }

    // // SigCapture sig;
    // string host_name, host_addr;
    // bool initialized;
};

// global shared memory
Hobo& global_gecko = gecko::Hobo::getInstance();

}; // end gecko namespace

namespace gecko {

static bool initialized = false;
mutex g_mutex;
static string host_name;
static string host_addr;
// static string mc_addr = {"224.3.29.110"};
// static int mc_port = 11311;
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
    printf(">> address: %s\n", addr.c_str());
    // if (file.empty()) addr = zmqTCP(host_addr);  // bind to next available port
    // else addr = zmqUDS(file);
    // string addr = zmqTCP(host_addr);  // bind to next available port
    T *p = new T();
    p->bind(addr);
    int retry = 5;

    BCSocket ss(global_gecko.mc_port);
    ss.bind();

    printf(">> made socket\n");

    Ascii a;
    pid_t pid = getpid();
    ascii_t tmp = {key,topic,to_string(pid),p->endpoint};
    string msg = a.pack(tmp);

    cout << "binder send: " << msg << endl;

    for (int i=0; i<retry; i++){
        ss.cast(msg);
        printf("binder send\n");

        string ans;
        struct sockaddr_in addr;
        tie(ans, addr) = ss.recv_nb();

        if(!ans.empty()){
            cout << "binder ans: " << ans << endl;
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
};

template<typename T>
T* connecter(string key, string topic){
    BCSocket ss(global_gecko.mc_port);
    // MCSocket(mc_addr, mc_port);
    ss.bind();
    int retry = 5;

    Ascii a;
    pid_t pid = getpid();
    ascii_t tmp = {key,topic, to_string(pid)};
    string msg = a.pack(tmp);

    for (int i=0; i<retry; i++){
        ss.cast(msg);
        printf("connect send\n");

        string ans = {""};
        struct sockaddr_in addr = {0};
        tie(ans, addr) = ss.recv();

        if(!ans.empty()){
            cout << "sub ans " << ans << ' ' << ans.size() << endl;
            ascii_t t = a.unpack(ans);
            if(t.size() == 4 && t[3] == "ok"){
                cout << "t.back() " << t.back() << endl;
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

// void gecko::init(string mc, int port){
void init(){
    // cout << "init: " << global_gecko.initialized << endl;

    // gecko::mc_port = iport;
    // global_gecko.mc_port = iport;

    // cout << "global_gecko address: " << &global_gecko << endl;
    // cout << "global_gecko address: " << global_gecko.getAddress() << endl;

    // cout << "iport: " << iport << endl;
    // cout << "mc_port: " << global_gecko.mc_port << endl;
    // lock_guard<mutex> guard(g_mutex);
    // if (initialized) return;

    printf("gecko --------------------------\n");
    printf(" singleton initialized: %s\n", global_gecko.initialized ? "true" : "false");
    printf(" %s [%s]\n", global_gecko.host_name.c_str(), global_gecko.host_addr.c_str());
    // printf(" multicast: %s:%d\n", mc_addr.c_str(), mc_port);
    printf(" broadcast port: %d\n", global_gecko.mc_port);
    // printf(" key: %s\n", key.c_str());
    printf(" signal handler enabled: %s\n", global_gecko.enabled ? "true" : "false");
    printf("\n");
}

// void hello(){
//     cout << "ehllo" << endl;
// }

} // gecko namespace end

int* gecko::getAddress(){
    return global_gecko.getAddress();
}

// void gecko::hello(){
//     cout << "ehllo" << endl;
// }

void gecko::set_broadcast(int port){
    global_gecko.mc_port = port;
}

int gecko::get_broadcast(){
    return global_gecko.mc_port;
}

bool gecko::ok(){
    if (global_gecko.enabled == false) throw GeckoCppError("SigCapture::on() not called");
    return global_gecko.ok;
}

void gecko::shutdown(){
    global_gecko.ok = false;
}

Subscriber* gecko::subBindTCP(string key, string topic){
    return binder<Subscriber>(key, topic, global_gecko.host_addr, zmqTCP);
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
    return binder<Publisher>(key, topic, global_gecko.host_addr, zmqTCP);
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
