// #include "gecko.hpp"
//
//
// using namespace std;
//
// void pub(bool *e){
//     Logger log;
//
//     Transport<imu_t> buffer;
//     try {
//         pid_t pid = getpid();
//         printf("-> pub pid: %d\n",pid);
//         Publisher *p = Node::advertise("local", "a");
//         if (p == nullptr){
//             printf("Pubisher failed\n");
//             return;
//         }
//         Rate r(30);
//
//         while(*e){
//             vec_t a(1,2,3);
//             imu_t b(a,a,a);  // new timestamp
//             zmq::message_t msg = buffer.pack(b);
//
//             p->pub(msg);
//             // printf(">> [PUB] sent msg\n");
//             r.sleep();
//
//             // log.loginfo("[PUB]\n  sent\n  msg\n");
//             // log.logwarn("[PUB] sent msg\n");
//             // log.logerror("[PUB] sent msg\n");
//             // log.logdebug("[PUB] sent msg\n");
//         }
//         delete p;
//     }
//     catch(zmq::error_t& e) {
//         cout << e.what() << endl;
//         exit(1);
//     }
//
//     printf(">> pub bye ...\n");
// }
//
//
//
//
// int main(void){
//     Node::init();
//
//     // Node p; p.run(pub);
//     // Node::wait();
//     pub(&Node::ok);
//
//     return 0;
// }
