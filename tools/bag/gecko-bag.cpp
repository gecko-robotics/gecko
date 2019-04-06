// https://github.com/msgpack/msgpack-c/issues/196

#include <msgpack.hpp>
#include <msgpack/fbuffer.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>


/*
 * map[topic][data vector]
 */
template <class T>
class my_class {
public:
    my_class(): revision(1) {}
    // std::vector<std::vector<T> > A, B;
    int revision;
    std::map<std::string, std::vector<std::vector<T> > > data;
    MSGPACK_DEFINE(revision, data); // write the member variables that you want to pack
    // my_class(){
    //    // A.resize(20, std::vector<T>(20, 300.123445435));
    //    revision = 123;
    //    // matrices.insert(std::pair<std::string, std::vector<std::vector<T> > >("T",A));
    //    // matrices.insert(std::pair<std::string, std::vector<std::vector<T> > >("P",A));
    // }
    void push(const std::string& topic, const T& val){
        auto search = data.find(topic);
        if (search == data.end()){
            std::vector<T> v;
            v.push_back(val);
            // data.insert(std::pair<std::string, std::vector<T> >(topic, v));
            // data.try_emplace(topic, v);
        }
        else {
            // search->second.push_back(val);
            // data[topic].push_back(val);
        }
    }
};

int main() {
    // This is target object.
    my_class<double> classy;
    for (int i=0; i<200; i++) classy.push("bob", 1.267*i);

    std::string filename = "data.msgpack.bag";

    // Serialize it. (THIS WORKS!)
    {
        std::ofstream ofs(filename);
        msgpack::pack(ofs, classy);
    } // automatically close here.

    // Deserialize the serialized data
    std::ifstream ifs(filename, std::ifstream::in);
    std::stringstream buffer;
    buffer << ifs.rdbuf();

    msgpack::unpacked upd;
    msgpack::unpack(upd, buffer.str().data(), buffer.str().size());
    std::cout << upd.get() << std::endl;
}
