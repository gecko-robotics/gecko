#include "geckocpp.hpp"
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <unistd.h>  // getopt
#include "transport.hpp"
#include "network.hpp"
#include "signals.hpp"
#include "time.hpp"
#include "helpers.hpp"

#include <thread>
#include <mutex>
#include "mbeacon.hpp"

using namespace std;

void beacon(bool& ok){
    Rate rate(10);
    while (ok){
        rate.sleep();
    }
}


/*
This holds the static info for this thread/process. User will never call this
but just access it via the functions below.
- host_name: bob.local
- host_addr: 1.2.3.4
- key: name in multicast message requests
*/
// class Singleton: public SigCapture {
class Singleton {
public:
    static Singleton& get() {
        // Since it's a static variable, if the class has already been created,
        // it won't be created again.
        // And it **is** thread-safe in C++11.
        static Singleton myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    Singleton(Singleton const&) = delete;             // Copy construct
    Singleton(Singleton&&) = delete;                  // Move construct
    Singleton& operator=(Singleton const&) = delete;  // Copy assign
    Singleton& operator=(Singleton &&) = delete;      // Move assign

    // Any other public methods.
    void print(){
        // cout << "[GeckoCore]--------------------------" << endl;
        // // cout << "  Status: " << ((core_found) ? "Core Found" : "Core Unknown") << endl;
        // // cout << "  Core: " << core_addr << endl;
        // cout << "  Host: " << host_addr << endl;
        printf("GeckoCore --------------------------\n");
        printf(" initialized: %s\n", initialized ? "true" : "false");
        printf(" %s [%s]\n", host_name.c_str(), host_addr.c_str());
        printf(" multicast: %s:%d\n", core_addr.c_str(), core_port);
        printf(" key: %s\n", key.c_str());
        printf("\n");
    }
    // string core_addr;  // address geckocore runs at
    string host_addr;  // address of the system this process/thread runs at
    string host_name;
    string core_addr;
    int core_port;
    string key;
    vector<Subscriber*> subs;
    bool initialized;  // has geckocore been initialized yet?
    // bool core_found;
    map<string,string> pubs;  // topics, endpoints
    bool ok;
    mutex g_pages_mutex;

protected:
    Singleton(): core_port(-1), initialized(false), ok(true) //, core_found(false)
    {
        cout << "GeckoCore: constructor" << endl;

        HostInfo h = HostInfo();
        host_addr = h.address;
        host_name = h.hostname;
    }

    ~Singleton() {
        ok = false;
        cout << "GeckoCore: destructor" << endl;
        for (auto const& p: subs) delete p;
        subs.clear();
    }
};

// args --------------------------
#include <map>

// typedef struct {
//     std::string program;
//     std::map<char, std::string> args;
// } args_t;

args_t processArgs(int argc,  char* argv[]){
    args_t args;
    args.program = argv[0];  // program name
    for (int i=1; i < argc; i+=2) {
        if (strlen(argv[i]) == 2){
            char key = argv[i][1];  // -key
            if (i+i < argc || key == 'h'){
                string value(argv[i+1]);
                args.args.insert_or_assign(key, value);
            }
            else
                args.args.insert_or_assign(key, "");
        }
        else {
            printf("*** invalid args: %s %s ***\n",argv[i],argv[i+1]);
        }
    }
    return args;
}
//---------------------------------

void gecko::init(int argc,  char* argv[]){
    std::lock_guard<std::mutex> guard(Singleton::get().g_pages_mutex);

    if (Singleton::get().initialized) return;

    args_t args = processArgs(argc, argv);
    init(args);
}

void gecko::init(args_t args){

    cout<<"Address geckocore: " << &Singleton::get() << endl;

    printf("\n*** FIXME ***\n");

    string help("\n"
    "./program -h -m name \n"
    "  c: multicast core address: 239.255.255.250\n"
    "  p: multicast core port: 12345\n"
    "  h: help \n"
    "  k: key name for pub/sub if different from hostname: bob\n"
    );

    for( auto const& [key, val] : args.args ) {
        switch(key){
        case 'c':
            Singleton::get().core_addr = val;
            break;
        case 'p':
            Singleton::get().core_port = stoi(val);
            break;
        case 'h':
            printf("%s\n", help.c_str());
            exit(0);
        case 'k':
            printf("Args: %c %s\n", key, val.c_str());
            Singleton::get().key = val;
            break;
        default:
            printf("*** Unknown args: %c %s ***\n",key,val.c_str());
        }
    }

    if (Singleton::get().core_addr.empty())
        Singleton::get().core_addr = "239.255.255.250";

    if (Singleton::get().core_port == -1)
        Singleton::get().core_port = 12345;

    if (Singleton::get().key.empty())
        Singleton::get().key = Singleton::get().host_name;

    Singleton::get().initialized = true;
    Singleton::get().print();
}

bool gecko::ok(){
    return Singleton::get().ok;
}

Publisher* gecko::advertise(string topic, int queue, bool bind){
    string key = Singleton::get().host_name; // arg


    string addr = zmqTCP(Singleton::get().host_addr);  // bind to next available port
    Publisher *p = new Publisher(addr, true);

    BeaconClient beacon;
    bool err = beacon.init(Singleton::get().core_addr, Singleton::get().core_port);
    if(err){
        printf("beacon::init error\n");
        return nullptr;
    }

    // stringstream ss;
    // ss << Singleton::get().host_name << "|" << topic << "|" << p->endpoint;

    int cnt = 50;
    bool find = true;
    while(find && cnt--)
    {
        printf("advertise[%d] sent\n", cnt);
        // string ans = beacon.find(ss.str());
        string ans = beacon.notify(key, topic, p->endpoint);

        if (ans.empty()){
            printf("advertise: Couldn't register endpoint for topic %s\n", topic.c_str());
            sleep(1);
        }
        else {
            printf(">> advertise got: %s\n", ans.c_str());
            Parser par;
            vector<string> v;
            bool err = par.parse(ans,v);

            printf("\n>> %s %lu\n",v[2].c_str(),v[2].size());
            printf("\n>> %s %lu\n",p->endpoint.c_str(),p->endpoint.size());

            // assert(v[0] == Singleton::get().host_name);
            // assert(v[1] == topic);
            // assert(v[2] == p->endpoint);
            // assert(v[3] == "ok");

            if((v[0] == Singleton::get().host_name) && (v[1] == topic) && (v[2] == p->endpoint) && (v[3] == "ok")){
                printf("<<<<<<< Pub notification successful >>>>>>>>\n");
                for(auto const& s: v) printf("> %s\n",s.c_str());
                find = false;
            }
            else {
                printf("advertise: Couldn't register endpoint for topic %s\n", topic.c_str());
                printf("\nDEBUG: %s\n\n",ans.c_str());

                for(auto const& s: v) printf("> %s\n",s.c_str());

                printf("\n* %s\n",Singleton::get().host_name.c_str());
                printf("* %s\n",topic.c_str());
                printf("* %s\n\n",p->endpoint.c_str());
            }
        }
    }
    if(find){
        printf("FAILED to register %s\n\n", topic.c_str());
        exit(1);
    }

    return p;
}

Subscriber* gecko::subscribe(string topic, void(*cb)(zmq::message_t&), int queue, bool bind){
    string key = Singleton::get().host_name; // arg

    BeaconClient beacon;
    bool err = beacon.init(Singleton::get().core_addr, Singleton::get().core_port);
    if(err){
        perror("subscribe::");
        return nullptr;
    }
    // stringstream ss;
    // ss << Singleton::get().host_name << "|" << topic;

    Subscriber *s = nullptr;
    int cnt = 5;
    bool find = true;
    while (find && cnt--){
        printf(">> subscribe sent: %s | %s\n", key.c_str(), topic.c_str());
        // string ans = beacon.find(ss.str());
        string ans = beacon.find(key, topic);

        if (ans.empty()){
            printf("Subscribe: Couldn't find endpoint for topic %s\n", topic.c_str());
        }
        else {
            printf(">> subscribe recv'd: %s\n", ans.c_str());
            Parser par;
            vector<string> v;
            bool err = par.parse(ans, v);
            printf(">> Subscribing to %s on %s\n", v[1].c_str(), v[2].c_str());
            s = new Subscriber(v[2].c_str(), topic, bind);
            if(cb != nullptr) s->setCallback(cb);
            Singleton::get().subs.push_back(s);
        }
        sleep(1);
    }
    return s;
}

void gecko::spin(int hertz){
    Rate rate(hertz);
    while(gecko::ok()){
        // for(int i=0; i < Singleton::get().subs.size(); ++i){
        //     zmq::message_t msg = Singleton::get().subs[i]->recv();
        // }
        for (auto const& p: Singleton::get().subs){
            zmq::message_t msg = p->recv_nb();
            if (p != nullptr) p->callback(msg);
        }
        rate.sleep();
    }

    // clean up
    // for(int i=0; i < Singleton::get().subs.size(); ++i) delete Singleton::get().subs[i];
    for (auto const& p: Singleton::get().subs) delete p;
    Singleton::get().subs.clear();
}

void gecko::loginfo(std::string msg){
    cout << "[INFO]: " << msg << endl;
}



//////////////////////////////////////////////////////////////////////////

// /*
//     core: tcp://x.x.x.x:port
//     return: found core true/false
// */
// bool pingCore(string& core){
//     Request req(core);
//     zmq::message_t msg("ping", 4);
//     zmq::message_t resp = req.get(msg);
//     string s(static_cast<char*>(resp.data()), resp.size());
//     // bool ret = false;
//     // if (s == "ok") ret = true;
//     // return ret;
//     return (s == "ok");
// }
//
// /*
//     core: x.x.x.x:port
//     topic: topic name
//     return: tcp://1.2.3.4:1234
// */
// string getTopic(string& core, string& topic){
//     Request req(core);
//     string msg = string("get:") + topic;
//     zmq::message_t m((void*)msg.data(), msg.size());
//
//     // resp = tcp://1.2.3.4:1234
//     zmq::message_t resp = req.get(m);
//     string addr(static_cast<char*>(resp.data()), resp.size());
//
//     cout << "getTopic: " << addr << endl;
//     return addr;
// }
//
// /**
//     core: tcp://1.2.3.4:1234
//     topic: topic name
//     return: success true/false
// */
// bool setTopic(const string& core, const string& topic){
//     Request req(core);
//
//     string msg = string("set:") + topic;
//     zmq::message_t m((void*)msg.data(), msg.size());
//
//     // resp = ok/fail
//     zmq::message_t resp = req.get(m);
//     string status(static_cast<char*>(resp.data()), resp.size());
//
//     return (status == "ok");
// }
// #include <type_traits>
// #include <typeinfo>
// #include <memory>
// #include <string>
// #include <cstdlib>
// template <typename T>
// std::string type_name(){
//     typedef typename std::remove_reference<T>::type TR;
//     std::unique_ptr<char, void(*)(void*)> own
//            (
//                 nullptr,
//                 std::free
//            );
//     std::string r = own != nullptr ? own.get() : typeid(TR).name();
//     if (std::is_const<TR>::value)
//         r += " const";
//     if (std::is_volatile<TR>::value)
//         r += " volatile";
//     if (std::is_lvalue_reference<T>::value)
//         r += "&";
//     else if (std::is_rvalue_reference<T>::value)
//         r += "&&";
//     return r;
// }

// #include <typeinfo>
