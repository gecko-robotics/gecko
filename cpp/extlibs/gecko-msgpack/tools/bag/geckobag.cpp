// https://github.com/msgpack/msgpack-c/issues/196

#include <msgpack.hpp>
#include <msgpack/fbuffer.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>

#include <gecko/msgpack/geckobag.hpp>

using namespace std;


/*
 * map[topic][data vector]
 */
// template <class T>
// class Bag {
// public:
//     Bag() {}
//     void write(const std::string& filename, bool timestamp=false) {}
//     template<class T>
//     void push(const std::string& topic, const T& msg) {}
//     void push_timestamp(const std::string& topic, const T& msg) {} // don't know
//     template
//
//     // std::vector<std::vector<T> > A, B;
//     // int revision;
//     // std::map<std::string, std::vector<std::vector<T> > > data;
//     // MSGPACK_DEFINE(revision, data); // write the member variables that you want to pack
//     // my_class(){
//     //    // A.resize(20, std::vector<T>(20, 300.123445435));
//     //    revision = 123;
//     //    // matrices.insert(std::pair<std::string, std::vector<std::vector<T> > >("T",A));
//     //    // matrices.insert(std::pair<std::string, std::vector<std::vector<T> > >("P",A));
//     // }
//     // void push(const std::string& topic, const T& val){
//     //     auto search = data.find(topic);
//     //     if (search == data.end()){
//     //         std::vector<T> v;
//     //         v.push_back(val);
//     //         // data.insert(std::pair<std::string, std::vector<T> >(topic, v));
//     //         // data.try_emplace(topic, v);
//     //     }
//     //     else {
//     //         // search->second.push_back(val);
//     //         // data[topic].push_back(val);
//     //     }
//     // }
//
// };

// struct Abstract {
//     // virtual ~Abstract() = 0;
//     std::vector<zmq::message_t> buffer;
// };
//
// template<class T>
// class Topic: public Abstract {
// public:
//     Topic() {}
//     void push(T msg) {
//         // zmq::message_t zm = packer.pack(msg);
//         buffer.push_back(packer.pack(msg));
//         cout << "push:" << endl;
//     }
//     // virtual ~Topic(){}
// protected:
//     MsgPack<T> packer;
//     // std::vector<zmq::message_t> buffer;
// };
//
// class Bag {
// public:
//     Bag() {}
//     template<class T> void newTopic(const std::string& topic){
//         Topic<T> *mp = new Topic<T>();
//         db[topic] = static_cast<Abstract*>(mp);
//         cout << "new topic" << endl;
//     }
//
//     template<class T>
//     void push(std::string topic, const T& msg){
//         Topic<T> *t = static_cast<Topic<T>*>(db[topic]);
//         t->push(msg);
//         cout << "push" << endl;
//     }
//
//     void write(std::string filename){
//         for (auto const& [topic, abs]: db){
//             cout << topic << endl;
//             for (auto const& msg: abs->buffer) cout << msg << endl;
//         }
//     }
//
// protected:
//     std::map<std::string, Abstract*> db;
// };

int main() {

    // Bag bag;
    // vec_t v(1,2,3);
    // cout << v << endl;
    // bag.newTopic<vec_t>("bob");
    // bag.push<vec_t>("bob", v);
    // bag.push<vec_t>("bob", v);
    // bag.push<vec_t>("bob", v);
    // bag.push<vec_t>("bob", v);
    // bag.write("tom");
    // // This is target object.
    // my_class<double> classy;
    // for (int i=0; i<200; i++) classy.push("bob", 1.267*i);
    //
    // std::string filename = "data.msgpack.bag";
    //
    // // Serialize it. (THIS WORKS!)
    // {
    //     std::ofstream ofs(filename);
    //     msgpack::pack(ofs, classy);
    // } // automatically close here.
    //
    // // Deserialize the serialized data
    // std::ifstream ifs(filename, std::ifstream::in);
    // std::stringstream buffer;
    // buffer << ifs.rdbuf();
    //
    // msgpack::unpacked upd;
    // msgpack::unpack(upd, buffer.str().data(), buffer.str().size());
    // std::cout << upd.get() << std::endl;
    return 0;
}
