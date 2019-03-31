#include <gtest/gtest.h> // googletest header file
#include "gecko.hpp"
#include "msgs.hpp"
#include <string>
#include <iostream>

using namespace std;


TEST(zmq, endpoint) {
    EXPECT_TRUE("tcp://1.2.3.4:*" == zzmqTCP("1.2.3.4").endpoint);
    EXPECT_TRUE("ipc://file" == zzmqUDS("file").endpoint);
}

TEST(zmq, pubsub) {
    // string uds = "ipc://test_uds_file";
    string uds = zzmqUDS("test_uds").endpoint;
    Transport<imu_t> buffer;

    // Subscriber s(uds, false);
    Publisher p(uds, true);
    Subscriber s(uds, false);

    vec_t a(1,2,3);
    imu_t b(a,a,a);
    // b.print();

    zmq::message_t msg = buffer.pack(b);
    zmq::message_t msgsave = buffer.pack(b);

    EXPECT_EQ(msgsave, msg);

    // zmq::message_t msg("hello", 5);
    // cout << "msg " << msg << endl;
    p.pub(msg);

    gecko::sleep(1);

    zmq::message_t ans = s.recv_nb();
    // cout << "ans " << ans << endl;
    EXPECT_EQ(msgsave, ans);

    if(ans.size() > 0){
        imu_t c = buffer.unpack(ans);
        EXPECT_EQ(b,c);
    }
    // else EXPECT_TRUE(false) << "*** No message received ***";
}

// TEST(zmq, subpub) {
//     // string uds = "ipc://test_uds_file";
//     string uds = zzmqUDS("test_uds").endpoint;
//     Transport<imu_t> buffer;
//
//     // Subscriber s(uds, false);
//     Publisher p(uds, false);
//     Subscriber s(uds, true);
//
//     vec_t a(1,2,3);
//     imu_t b(a,a,a);
//     // b.print();
//
//     for (int i=0; i < 1; i++){
//         zmq::message_t msg = buffer.pack(b);
//         // zmq::message_t msg("hello", 5);
//         // cout << "msg " << msg << endl;
//         p.pub(msg);
//     }
//
//     gecko::sleep(1);
//
//     zmq::message_t ans = s.recv_nb();
//     // cout << "ans " << ans << endl;
//
//     if(ans.size() > 0){
//         imu_t c = buffer.unpack(ans);
//         EXPECT_EQ(b,c);
//     }
//     else EXPECT_TRUE(false) << "*** No message received ***";
// }
