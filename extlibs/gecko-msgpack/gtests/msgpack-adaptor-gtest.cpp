#include <gtest/gtest.h> // googletest header file
#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>
// #include "gecko/msgpack/msgpack_pub_sub.hpp"
#include <string>

using namespace std;

// TEST(dummy, simple) {
//     ASSERT_TRUE(true);
// }


void ps(std::string path){
    gecko::init();

    vec_t a(1,-2,3.3);

    Publisher p;
    p.bind(path);

    Subscriber s;
    s.connect(path);

    // we always loose the first message due to subscription time
    while (1){
        zmq::message_t mm = a.pack();
        p.publish(mm);
        gecko::msleep(100);
        zmq::message_t msg = s.recv_nb();
        if (msg.size() > 0){
            try{
                vec_t b(msg);
                ASSERT_TRUE(a == b);
            }
            catch (exception& e){
                cout << "wtf: " << e.what() << endl;}
            // ASSERT_TRUE(a == b);
            break;
        }
    }
}

TEST(msgpack, gecko_pub_sub_uds) {
    ps(zmqUDS("/tmp/0"));
}

TEST(msgpack, gecko_pub_sub_tcp) {
    HostInfo h;
    // ps(zmqTCP(h.address,9000));
}

void rr(std::string path){
    ASSERT_TRUE(true);
}

TEST(msgpack, gecko_req_rep_uds) {
    rr(zmqUDS("/tmp/0"));
}

TEST(msgpack, gecko_req_rep_tcp) {
    HostInfo h;
    rr(zmqTCP(h.address,9000));
}
