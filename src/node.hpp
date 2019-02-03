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

    static Publisher* advertise(
            const std::string& key,
            const std::string& topic,
            int queue=1,
            bool bind=true){
        // ensure only 1 thread writes to db at a time
        std::lock_guard<std::mutex> guard(g_mutex);

        std::string addr = zmqTCP(host_addr);  // bind to next available port
        Publisher *p = new Publisher(addr, bind);

        db.push(key, topic, p->endpoint);
        printf(">> advertise: %s on %s [%s]\n",
            topic.c_str(),
            key.c_str(),
            p->endpoint.c_str());

        return p;
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

    static void wait(){
        while(ok) {sleep(1);}
    }

    static bool initialized;
    static Directory db;
    static std::string host_name;
    static std::string host_addr;

protected:
    std::thread the_thread;
    static std::mutex g_mutex;
};
