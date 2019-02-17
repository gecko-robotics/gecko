#include "transport.hpp"
#include "zmq.hpp"
#include "time.hpp"
#include "color.hpp"
#include <iostream>
// #include <algorithm>  // erase

using namespace std;

// enum zmq_types_t {pub, sub, rep, req};
// enum class socket_type : int
// {
//     req = ZMQ_REQ,
//     rep = ZMQ_REP,
//     // dealer = ZMQ_DEALER,
//     // router = ZMQ_ROUTER,
//     pub = ZMQ_PUB,
//     sub = ZMQ_SUB,
//     // xpub = ZMQ_XPUB,
//     // xsub = ZMQ_XSUB,
//     // push = ZMQ_PUSH,
//     // pull = ZMQ_PULL,
//     // pair = ZMQ_PAIR
//     // std::string to_str(){
//     //
//     // }
// };

// string zmqTCP(string addr, string port){
string zmqTCP(const string& addr){
    // return fmtstr("tcp://%s:%s", addr.c_str(), port.c_str());
    // return std::string("tcp://") + addr + std::string(":") + port;
    // stringstream ss;
    // ss << "tcp://" << addr << ":" << port;
    // printf("%s\n", ss.str().c_str());
    // return ss.str();

    string endpt = "tcp://";
    endpt += addr;
    endpt += ":*";  // get next available port
    // endpt += port;
    printf("zmqTCP: %s\n", endpt.c_str());
    return endpt;
}

string zmqUDS(const string& file){
    // return fmtstr("tcp://%s:%s", addr.c_str(), port.c_str());
    // return std::string("tcp://") + addr + std::string(":") + port;
    // stringstream ss;
    string endpt = "ipc://";
    endpt += file;
    printf("zmqUDS: %s\n", endpt.c_str());
    return endpt;
}

std::string zmqType::to_string() {
    std::string ans;
    ColorPrint c;
    switch(type){
    case ZMQ_PUB:
        ans = c.color(0,6,"ZMQ_PUB");
        break;
    case ZMQ_SUB:
        ans = c.color(0,7,"ZMQ_SUB");
        break;
    case ZMQ_REQ:
        ans = c.color(0,2,"ZMQ_REQ");
        break;
    case ZMQ_REP:
        ans = c.color(0,3,"ZMQ_REP");
        break;
    default:
        ans = c.color(0,1,"ZMQ_ZMQ_UNKNOWNSUB");
    }
    return ans;
}

const char* zmqType::c_str(){
    return to_string().c_str();
}


zmq::context_t zmqBase::gContext(1);
zmqBase::zmqBase(int t): type(t), sock(gContext, t), bind(false) {}

/*
typedef struct
{
    void* //socket//;
    int //fd//;
    short //events//;
    short //revents//;
} zmq_pollitem_t;
*/
bool zmqBase::check(uint16_t msec){
    zmq_pollitem_t items[] = {
      {sock, 0, ZMQ_POLLIN, 0},
    };

    // if (zmq_poll(items, 1, msec) > 0) return true;
    int ret = zmq_poll(items, 1, msec);
    if (ret < 0) perror("zmq_poll failed");
    else if (ret > 0) return true;
    return false;
}
// bool zmqBase::check(int retry){
//     zmq_pollitem_t items[] = {
//       {sock, 0, ZMQ_POLLIN, 0},
//     };
//
//     for (int count = retry; count > 0; --count){
//         int ret = zmq_poll(items, 1, 10);  // wait 10 msec
//         if (ret < 0) return false;
//         else if (ret > 0) return true;
//     }
//     return false;
// }

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
    // // any pending sends will block the context destructor
    // cout << ">> killing (ZMQ_LINGER): " << endpoint << endl;
    // if(bind) sock.unbind(endpoint);
    // int msec = 5;
    // sock.setsockopt(ZMQ_LINGER, &msec, sizeof(msec));
    // sock.close();
    zmqBase::close();
}

void zmqBase::close(){
    // any pending sends will block the context destructor
    zmqType z(type);
    printf(">> %s killing (ZMQ_LINGER): %s\n",z.c_str(), endpoint.c_str());
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
    // if(check()) sock.recv(&msg, flags);
    sock.recv(&msg, flags);
    return msg;
}


void Subscriber::setCallback(void(*cb)(zmq::message_t&)){
    callback = cb;
}

////////////////////////////////////////////////////

Reply::Reply(std::string addr): zmqBase(ZMQ_REP){
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

Request::Request(std::string addr): zmqBase(ZMQ_REQ){
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
