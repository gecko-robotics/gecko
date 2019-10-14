#include "gecko/gecko.hpp"

using namespace std;

int main(){
    string key = "local";
    string topic = "log";

    gecko::init();

    Subscriber *s = gecko::subBindTCP(key, topic);
    // Subscriber *s = gecko::subBindUDS(key, topic, "test_uds_file");
    if (s == nullptr) {
        gecko::shutdown();
        return 1;
    }

    Rate r(20);

    while(gecko::ok()){
        zmq::message_t m = s->recv_nb();
        if (m.size() > 0) gecko::log(gecko::DEBUG, "got message\n");
        r.sleep();
    }

    delete s;

    return 0;
}
