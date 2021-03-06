#include <gtest/gtest.h> // googletest header file
#include "gecko/gecko.hpp"
// #include "msgpack/msgs.hpp"
#include <string>
#include <iostream>

using namespace std;


TEST(zmq, endpoint) {
    EXPECT_TRUE("tcp://1.2.3.4:*" == zmqTCP("1.2.3.4"));
    EXPECT_TRUE("tcp://1.2.3.4:9000" == zmqTCP("1.2.3.4",9000));
    EXPECT_TRUE("ipc://file" == zmqUDS("file"));
}

TEST(zmq, pub_sub_uds) {
    // string uds = "ipc://test_uds_file";
    string uds = zmqUDS("/tmp/test_uds");

    Publisher p;
    p.bind(uds);

    string str = "helloworld";
    zmq::message_t msg(static_cast<void*>(str.data()), str.size());
    zmq::message_t msgsave(static_cast<void*>(str.data()), str.size());

    EXPECT_EQ(msgsave, msg);

    Subscriber s;
    s.connect(uds);

    // we always loose the first message due to subscription time
    zmq::message_t ans;
    while (ans.size() == 0){
        zmq::message_t mm(static_cast<void*>(str.data()), str.size());
        p.publish(mm);
        gecko::msleep(100);
        ans = s.recv_nb();
        // cout << ans << endl;
    }

    EXPECT_EQ(msgsave.size(), ans.size());
    EXPECT_EQ(msgsave, ans);

    string str2(reinterpret_cast<const char*>(ans.data()), ans.size());
    EXPECT_TRUE(str == str2);
}


zmq::message_t f(zmq::message_t& m){
    // zmq::message_t ans;
    cout << ">> f msg: " << m << endl;

    string str(reinterpret_cast<const char*>(m.data()), m.size());
    if (str == "helloworld"){
        string str2 = "good";
        zmq::message_t ans(static_cast<void*>(str2.data()), str2.size());
        return ans;
    }
    zmq::message_t ret;
    return ret;
}

// seems not to work
// TEST(zmq, req_rep_uds) {
//     string uds = zmqUDS("/tmp/test_uds");
//     string str = "helloworld";
//
//     Reply rep(uds);
//     // rep.bind(uds);
//
//     Request req(uds);
//     // req.connect(uds);
//
//     zmq::message_t ans;
//     while(ans.size() == 0) {
//         zmq::message_t mm(static_cast<void*>(str.data()), str.size());
//         ans = req.get_nb(mm);
//         cout << ">> TEST() ans: " << ans << endl;
//         if (ans.size() > 0){
//             string ret(reinterpret_cast<const char*>(ans.data()), ans.size());
//             cout << "TEST() ret: " << ret << endl;
//             EXPECT_TRUE(ret == "good");
//             break;
//         }
//         gecko::msleep(100);
//         rep.listen_nb(f);
//     }
// }

// TEST(zmq, pubsubmsgs) {
//     // string uds = "ipc://test_uds_file";
//     string uds = zmqUDS("/tmp/test_uds");
//     MsgPack<imu_st> buffer;
//
//     // Subscriber s(uds, false);
//     Publisher p;
//     p.bind(uds);
//
//     vec_t a(1,2,3);
//     imu_st b(a,a,a);
//
//     zmq::message_t msg = buffer.pack(b);
//     zmq::message_t msgsave = buffer.pack(b); // msg is destroyed after sending
//
//     EXPECT_EQ(msgsave, msg);
//
//     Subscriber s;
//     s.connect(uds);
//
//     // we always loose the first message due to subscription time
//     zmq::message_t ans;
//     while (ans.size() == 0){
//         zmq::message_t mm = buffer.pack(b);
//         p.publish(mm);
//         gecko::msleep(100);
//         ans = s.recv_nb();
//         // cout << ans << endl;
//     }
//
//     EXPECT_EQ(msgsave, ans);
//
//     if(ans.size() > 0){
//         imu_st c = buffer.unpack(ans);
//         EXPECT_EQ(b,c);
//     }
//     // else EXPECT_TRUE(false) << "*** No message received ***";
// }

// TEST(zmq, subpub) {
//     // string uds = "ipc://test_uds_file";
//     string uds = zzmqUDS("test_uds").endpoint;
//     MsgPack<imu_t> buffer;
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
