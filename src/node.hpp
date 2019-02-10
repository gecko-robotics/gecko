#pragma once

#include <thread>
#include <mutex>
#include <string>
#include <unistd.h>       // sleep
#include "signals.hpp"    // SigCapture
#include "directory.hpp"  // db
#include "transport.hpp"  // pub/sub
#include "network.hpp"    // hostinfo
#include "helpers.hpp"    // zmqtTCP
#include "ascii.hpp"
#include "msocket.hpp"


class Process {
public:
    Process(): sid(0) {}
    void go_daemon();
    void savepid(const std::string& fname);
    pid_t sid;
};

/*
Spins off a thread for a given function
*/
// class Threaded: protected SigCapture {
// public:
//     void run(void(*f)(void*));
// };

// // what is the value of this?
// class Node {
// public:
//     Node();
// };

// class ThreadedNode: public Node, public Threaded {};
//
// };

// thread class member
// https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class Node: public SigCapture {
public:
    // Node(): mc_addr("224.3.29.110"), mc_port(11311) {}
    ~Node();
    void run(void(*f)(bool*));

    inline std::thread::id getId(){
        return the_thread.get_id();
    }

    // static functions ---------------------------------
    static void init(){
        dir_map_t dm;
        init(dm);
    }

    //
    static void init(dir_map_t& dm){
        std::lock_guard<std::mutex> guard(g_mutex);
        if (initialized) return;

        for (auto const& [key,dir]: dm){
            for (auto [topic,endpt]: dir){
                db.push(key, topic, endpt);
            }
        }
        db.print();

        HostInfo h = HostInfo();
        host_addr = h.address;
        host_name = h.hostname;

        initialized = true;
        printf("Node --------------------------\n");
        printf(" initialized: %s\n", initialized ? "true" : "false");
        printf(" %s [%s]\n", host_name.c_str(), host_addr.c_str());
        // printf(" multicast: %s:%d\n", core_addr.c_str(), core_port);
        // printf(" key: %s\n", key.c_str());
        printf("\n");
    }

    // static Publisher* advertise(
    //         const std::string& key,
    //         const std::string& topic,
    //         int queue=1,
    //         bool bind=true){
    //     // ensure only 1 thread writes to db at a time
    //     std::lock_guard<std::mutex> guard(g_mutex);
    //
    //     std::string addr = zmqTCP(host_addr);  // bind to next available port
    //     Publisher *p = new Publisher(addr, bind);
    //
    //     db.push(key, topic, p->endpoint);
    //     printf(">> advertise: %s on %s [%s]\n",
    //         topic.c_str(),
    //         key.c_str(),
    //         p->endpoint.c_str());
    //
    //     return p;
    // }
    static Publisher* advertise(std::string key, std::string topic, int pid, int retry=5){
        std::string addr = zmqTCP(host_addr);  // bind to next available port
        Publisher *p = new Publisher(addr, true);

        SSocket ss;
        ss.init("224.3.29.110", 11311);

        Ascii a;
        ascii_t tmp = {key,topic,std::to_string(12345),p->endpoint};
        std::string msg = a.pack(tmp);

        for (int i=0; i<retry; i++){
            ss.send(msg);
            printf("pub send\n");
            std::string ans = ss.recv();

            if(!ans.empty()){
                ascii_t t = a.unpack(ans);
                if(t.size() == 4){
                    if (t.back() == "ok") {
                        std::string topic = t[1];
                        printf(">> PUB[%s]: %s\n", topic.c_str(), t.back().c_str());
                        return p;
                    }
                    // else std::cout << "** pub t.back() " << t.back() << std::endl;
                }
            }
        }
        delete p;
        return nullptr;
    }

    static Subscriber* subscribe(
            const std::string& key,
            const std::string& topic,
            int queue=1,
            bool bind=false){

        Subscriber *s = nullptr;

        while(s == nullptr){
            std::string endpt = db.find(key, topic);
            if(endpt.empty()) sleep(1);
            else s = new Subscriber(endpt, bind);
        }
        return s;
    }

    static Subscriber* subscribe2(std::string key, std::string topic, int pid, int retry=5){
        SSocket ss;
        ss.init("224.3.29.110", 11311);

        Ascii a;
        ascii_t tmp = {key,topic, std::to_string(pid)};
        std::string msg = a.pack(tmp);

        for (int i=0; i<retry; i++){
            ss.send(msg);
            printf("sub send\n");
            std::string ans = ss.recv();
            // cout << "sub ans " << ans << ' ' << ans.size() << endl;

            if(!ans.empty()){
                ascii_t t = a.unpack(ans);
                if(t.size() == 3){
                    // cout << "t.back() " << t.back() << endl;
                    if (t[0] == key && t[1] == topic) {
                        std::string endpt = t[2];
                        printf(">> SUB[%s]: %s\n",topic.c_str(), endpt.c_str());

                        return new Subscriber(endpt, false);
                    }
                    // else cout << "** invalid ans " << ans << endl;
                }
            }
        }

        return nullptr;
    }

    static void wait(){
        while(ok) {sleep(1);}
    }

    static bool initialized;
    static Directory db;
    static std::string host_name;
    static std::string host_addr;
    static std::string mc_addr;
    static int mc_port;

protected:
    std::thread the_thread;
    static std::mutex g_mutex;
};
