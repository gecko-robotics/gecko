
#include "geckocpp.hpp"
#include <thread>
#include <pwd.h>  // geteuid,getpwuid
#include <sys/types.h>  // umask
#include <sys/stat.h>  // umask
#include <iostream>
#include "log.hpp"
#include "signals.hpp"


using namespace std;


#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

namespace gecko {
    static bool initialized = false;
    mutex g_mutex;
    static string host_name;
    static string host_addr;
    static string mc_addr = {"224.3.29.110"};
    static int mc_port = 11311;
    Time time;
    Logger logger;
    static SigCapture sig;
}


bool gecko::ok(){
    return sig.ok;
}

void gecko::init(string mc, int port){
    lock_guard<mutex> guard(g_mutex);
    if (initialized) return;

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

Publisher* gecko::advertise(string key, string topic, int retry){
    string addr = zmqTCP(host_addr);  // bind to next available port
    Publisher *p = new Publisher(addr, true);

    SSocket ss;
    ss.init(mc_addr, mc_port);

    Ascii a;
    pid_t pid = getpid();
    ascii_t tmp = {key,topic,to_string(pid),p->endpoint};
    string msg = a.pack(tmp);

    for (int i=0; i<retry; i++){
        ss.send(msg);
        printf("pub send\n");
        string ans = ss.recv(900);

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            if(t.size() == 4){
                if (t.back() == "ok") {
                    string topic = t[1];
                    printf(">> PUB[%s]: %s\n",
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

Subscriber* gecko::subscribe(string key, string topic, int retry){
    SSocket ss;
    ss.init(mc_addr, mc_port);

    Ascii a;
    pid_t pid = getpid();
    ascii_t tmp = {key,topic, to_string(pid)};
    string msg = a.pack(tmp);

    for (int i=0; i<retry; i++){
        ss.send(msg);
        printf("sub send\n");
        string ans = ss.recv();
        // cout << "sub ans " << ans << ' ' << ans.size() << endl;

        if(!ans.empty()){
            ascii_t t = a.unpack(ans);
            if(t.size() == 3){
                // cout << "t.back() " << t.back() << endl;
                if (t[0] == key && t[1] == topic) {
                    string endpt = t[2];
                    printf(">> SUB[%s]: %s\n",topic.c_str(), endpt.c_str());

                    return new Subscriber(endpt, false);
                }
                // else cout << "** invalid ans " << ans << endl;
            }
        }
    }

    return nullptr;
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


// void gecko::wait(){
//     while(ok) {sleep(1);}
// }

// void gecko::wait(uint16_t sec){
//     for (int i=sec; i>0; i--) {sleep(1);}
//     ok = false;
// }

// std::thread::id gecko::getId(){
//     return the_thread.get_id();
// }
