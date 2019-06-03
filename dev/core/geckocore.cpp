#include <iostream>
#include <stdlib.h>
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <exception>

using namespace std;

struct InvalidKey : public std::exception {
    InvalidKey(const std::string &s): msg("Invalid Key: " + s) {}
    InvalidKey(): msg("Invalid Key") {}
    const char * what () const throw () {return msg.c_str();}
protected:
    std::string msg;
};

class DB {
public:
    DB(){}
    std::tuple<std::string, std::string> get(const std::string& topic);
    void push(const std::string& topic, const std::string& addr, const std::string& pid);
    void pop(const std::string& topic);
    int size(){ return db.size(); }
    void print();

// protected:
    // [topic, (addr, pid)]
    map<std::string, std::tuple<std::string, std::string>> db;
};

void DB::print(){
    printf("--------------------\n");
    for (auto const& [key, val] : db) {
        string addr;
        string pid;
        tie(addr, pid) = val;
        printf(" > %-10s %-15s %-10s \n",key.c_str(),addr.c_str(),pid.c_str());
    }
}

std::tuple<std::string, std::string> DB::get(const std::string& topic){
    map<string, tuple<string, string>>::iterator it;
    it = db.find(topic);
    if (it == db.end()) throw InvalidKey(topic);
    return it->second;
}

void DB::pop(const std::string& topic){
    db.erase(topic);
}

void DB::push(const std::string& topic, const std::string& addr, const std::string& pid){
    db[topic] = tuple(addr, pid);
}

int main(){
    DB db;

    db.push("topic", "tcp://hi:123", "12345");
    db.push("topic2", "tcp://hi:123", "12345");
    db.push("topic2", "tcp://hi:666", "666");  // over writes above
    db.push("topic23", "tcp://hi:666", "666"); // pop out soon

    db.print();

    db.pop("topic23");

    db.print();

    string a, p;
    tie(a,p) = db.get("topic");
    cout << a << " " << p << endl;

    try {
        tie(a,p) = db.get("fail");
        cout << a << " " << p << endl;
    }
    catch(exception& e){
        cout << "error " << e.what() << endl;
    }

    // tie(a,p) = db.get("this will fail");
    // cout << a << " " << p << endl;

    return 0;
}
