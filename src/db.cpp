#include <gecko/db.hpp>

using namespace std;


void DB::print(){
    printf("--------------------\n");
    for (auto const& [key, val] : db) {
        string addr;
        string pid;
        tie(addr, pid) = val;
        printf(" > %-10s %-15s %-10s \n",key.c_str(),addr.c_str(),pid.c_str());
    }
}

tuple<string, string> DB::get(const string& topic){
    map<string, tuple<string, string>>::iterator it;
    it = db.find(topic);
    if (it == db.end()) throw InvalidKey(topic);
    return it->second;
}

void DB::pop(const string& topic){
    db.erase(topic);
}

void DB::push(const string& topic, const string& addr, const string& pid){
    db[topic] = tuple(addr, pid);
}
