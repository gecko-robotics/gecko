#include <gecko/ps.hpp>
#include <gecko/db.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;


// void DB::print(){
//     printf("--------------------\n");
//     for (auto const& [key, val] : db) {
//         string addr;
//         string pid;
//         tie(addr, pid) = val;
//         printf(" > %-10s %-15s %-10s \n",key.c_str(),addr.c_str(),pid.c_str());
//     }
// }
//
// tuple<string, string> DB::get(const string& topic){
//     map<string, tuple<string, string>>::iterator it;
//     it = db.find(topic);
//     if (it == db.end()) throw InvalidKey(topic);
//     return it->second;
// }
//
// void DB::pop(const string& topic){
//     db.erase(topic);
// }
//
// void DB::push(const string& topic, const string& addr, const string& pid){
//     db[topic] = tuple(addr, pid);
// }


//////////////////////////////////////////

void DBs::print(){
    printf("--------------------\n");
    for (auto const& [key, val] : db) {
        printf(" > %-10s %-15s\n", key.c_str(), val.c_str());
    }
}

void DBs::printPs(){
    printf("--------------------\n");
    PS ps;
    vector<string> bad;
    for (auto const& [key, val] : db) {
        ps.get(val);
        if (ps.cpu.size() > 0) {
            cout << " > " << setfill('.') << setw(30) << left << key + "[" + val + "]" << "cpu: " << ps.cpu << " mem: " << ps.mem << endl;
        }
        else {
            bad.push_back(key);
        }
    }

    for (auto const& key: bad) pop(key);
}

string DBs::get(const string& key){
    map<string, string>::iterator it;
    it = db.find(key);
    if (it == db.end()) throw InvalidKey(key);
    return it->second;
}

void DBs::pop(const string& key){
    db.erase(key);
}

void DBs::push(const string& key, const string& val){
    db[key] = val;
}
