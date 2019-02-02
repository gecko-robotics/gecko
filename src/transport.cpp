#include "transport.hpp"
#include "zmq.hpp"
#include "time.hpp"
// #include <algorithm>  // erase

using namespace std;

zmq::context_t zmqBase::gContext(1);
zmqBase::zmqBase(int type): sock(gContext, type), bind(false) {}

/*
typedef struct
{
    void* //socket//;
    int //fd//;
    short //events//;
    short //revents//;
} zmq_pollitem_t;
*/
bool zmqBase::check(int retry){
    zmq_pollitem_t items[] = {
      {sock, 0, ZMQ_POLLIN, 0},
    };

    for (int count = retry; count > 0; --count){
        int ret = zmq_poll(items, 1, 10);  // wait 10 msec
        // cout << count << endl;
        if (ret < 0){
            cout << "zmqBase::check failed: " << ret << endl;
            return false;
        }
        if (ret > 0) return true;
        // Time::msleep(1);
    }
    return false;
}

void zmqBase::setEndPt(){
    // std::array<char, 100> e;  // tcp://x.x.x.x:port
    // size_t e_size = e.size();
    // sock.getsockopt(ZMQ_LAST_ENDPOINT, e.data(), &e_size);
    size_t s = 100;
    char e[s];
    sock.getsockopt(ZMQ_LAST_ENDPOINT, e, &s);

    endpoint = e;
    // std::cout << ">> endpoint: " << endpoint << std::endl;
}

zmqBase::~zmqBase(){
    // any pending sends will block the context destructor
    cout << ">> killing (ZMQ_LINGER): " << endpoint << endl;
    if(bind) sock.unbind(endpoint);
    int msec = 5;
    sock.setsockopt(ZMQ_LINGER, &msec, sizeof(msec));
    sock.close();
}

///////////////////////////////////////////////////////////////

Publisher::Publisher(): zmqBase(ZMQ_PUB){}

/*
Will bind or connect to an address (tcp://x.x.x.x:*, where * can be replacced
with a port number if desired)
https://stackoverflow.com/questions/16699890/connect-to-first-free-port-with-tcp-using-0mq
*/
Publisher::Publisher(string addr, bool bind): zmqBase(ZMQ_PUB)
{
    if (bind) {
        this->bind = true;
        // https://github.com/pi-hole/FTL/blob/master/socket.c#L63
        // setsockopt(int option_, const void *optval_, size_t optvallen_)
        // sock.setsockopt(SO_REUSEADDR, &(int){ 1 }, sizeof(int));
        sock.bind(addr);
    }
    else sock.connect(addr);

    setEndPt();
    printf(">> Publisher[%s] %s\n",endpoint.c_str(), bind ? "bind" : "connect");
}

void Publisher::pub(zmq::message_t& msg){
    sock.send(msg);
}

///////////////////////////////////////////////////

Subscriber::Subscriber(): zmqBase(ZMQ_SUB), callback(nullptr) {}

Subscriber::Subscriber(string addr, string topic, bool bind): zmqBase(ZMQ_SUB)
{
    if (bind) {
        this->bind = true;
        sock.bind(addr);
    }
    else sock.connect(addr);

    sock.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
    // callback = nullptr;
    setEndPt();
    printf(">> Subscriber[%s] %s\n",endpoint.c_str(), bind ? "bind" : "connect");
}

Subscriber::Subscriber(string addr, bool bind): zmqBase(ZMQ_SUB)
{
    if (bind) sock.bind(addr);
    else sock.connect(addr);

    sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    // callback = nullptr;
    setEndPt();
    printf(">> Subscriber[%s] %s\n",endpoint.c_str(), bind ? "bind" : "connect");
}

zmq::message_t Subscriber::recv(int flags){
    zmq::message_t msg;
    sock.recv(&msg, flags);
    return msg;
}


void Subscriber::setCallback(void(*cb)(zmq::message_t&)){
    callback = cb;
}

////////////////////////////////////////////////////

Reply::Reply(std::string addr): zmqBase(ZMQ_REP) {
    sock.bind(addr);
    setEndPt();
}

void Reply::listen(zmq::message_t (*callback)(zmq::message_t&), int flags){
    zmq::message_t request;

    if (zmqBase::check(1) == false) return;

    sock.recv (&request, flags);

    // if (request.size() == 0) return;

    //  create the reply
    zmq::message_t reply = callback(request);
    printf(">> sending reply\n");
    cout << reply << endl;
    if (reply.size() > 0) sock.send(reply);
}


////////////////////////////////////////////////////

Request::Request(std::string addr): zmqBase(ZMQ_REQ)  {
    sock.connect(addr);
    setEndPt();
}

zmq::message_t Request::get(zmq::message_t& msg, int flags){
    sock.send(msg);

    bool msg_ready = zmqBase::check(1);

    zmq::message_t rep;
    if (msg_ready) sock.recv(&rep, flags);

    return rep;
}
