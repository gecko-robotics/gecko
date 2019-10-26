#include <iostream>
#include <stdio.h>

#include <gecko/gecko.hpp>
// #include <msgs.pb.h>
#include <helper_pb.h>


using namespace std;
using namespace gecko;
using namespace geckopb;

// template <class MSG>
// zmq::message_t protobufPack(MSG& m){
//     string s;
//     m.SerializeToString(&s);
//     zmq::message_t msg(static_cast<void*>(s.data()), s.size());
//     return std::move(msg);
// }

int main(void){
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    gecko::init();
    try {
        HostInfo host;
        Publisher p;
        p.bind(zmqUDS("/tmp/simple-uds"));

        Rate r(1);

        while(gecko::ok()){
            Vector v;
            v.set_x(1.0);
            v.set_y(-0.00001);
            v.set_z(1000000.0);

            // string s;
            // v.SerializeToString(&s);
            // zmq::message_t msg(static_cast<void*>(s.data()), s.size());

            zmq::message_t msg = protobufPack<Vector>(v);

            p.publish(msg);
            // cout << ">> [PUB] sent msg[" << s.size() << "]: " << s << endl;
            cout << v.DebugString() << endl;
            r.sleep();
        }
    }
    catch(zmq::error_t& e) {
        cout << e.what() << endl;
        // exit(1);
    }

    printf(">> pub bye ...\n");

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
