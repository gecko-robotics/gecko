#include <gtest/gtest.h> // googletest header file
#include "gecko.hpp"
#include "msgs.hpp"
#include "msgpack_pub_sub.hpp"
#include <string>

using namespace std;

TEST(msgpack, msg_adaptor) {
    string uds = zmqUDS("/tmp/0");

    vec_t a(1,-2,3.3);

    Publisher *p = new Publisher();
    p->bind(uds);
    mpPublisher<vec_t> pub(p);
    pub.publish(a);

    Subscriber *s = new Subscriber("");
    s->connect(uds);
    mpSubscriber<vec_t> sub(s);
    vec_t b = sub.recv_nb();

    a.print();
    b.print();

    ASSERT_TRUE(a == b);
}
