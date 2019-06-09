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

//////////////////////////////////////////////



// void DBv::print(){
//     printf("--------------------\n");
//     // for (auto const& [key, val] : db) {
//     //     printf(" > %-10s %-15s\n", key.c_str(), val.c_str());
//     // }
// }

void DBv::print(){
    printDB(0, bind);
    printDB(1, conn);
}

/*
 Binders [6]
  [PID]   Topic                  CPU   MEM   EndPoint
  [65993] hello................. 0.0%  0.0%  tcp://10.0.1.57:65303
  [65994] hey there............. 0.0%  0.0%  tcp://10.0.1.57:65303
  [66008] ryan.................. 0.1%  0.0%  uds:///var/run/ryan-0
 Connections [8]
  [65995] hello................. cpu: 20.7%  mem:  0.0%
  [65996] hello................. cpu: 20.9%  mem:  0.0%
  [65997] hey there............. cpu: 21.0%  mem:  0.0%
*/
void DBv::printDB(int type, std::map<std::string, record_t>& db){
    if (db.size() == 0) return;
    string header;

    if (type == 0) header = "Binder";
    else header = "Connector";

    printf("--------------------\n");
    printf(" %s [%lu]\n", header.c_str(), db.size());
    printf(" %-31s %-4s %-4s %s\n", "Topic", "CPU", "MEM", "EndPt");
    PS ps;
    vector<string> bad;
    for (auto const& [key, val] : db) {
        string topic, pid, endpt;

        if (type == 0) {
            topic = key;
            tie(pid, endpt) = val;
        }
        else {
            pid = key;
            tie(topic, endpt) = val;
        }

        ps.get(pid);
        if (ps.cpu.size() > 0) {
            cout << " > " << setfill('.') << setw(30) << left << topic + "[" + pid + "]" << setfill(' ') << setw(5) << ps.cpu << setw(5) << ps.mem << endpt << endl;
        }
        else {
            bad.push_back(key);
        }
    }

    for (auto const& key: bad) pop(db, key);
}

string DBv::get(const string& key){
    map<string, record_t>::iterator it;
    it = bind.find(key);
    if (it == bind.end()) throw InvalidKey(key);
    string pid, endpt;
    tie(pid, endpt) = it->second;
    return endpt;
}

void DBv::pop(std::map<std::string, record_t>& db, const string& key){
    db.erase(key);
}

void DBv::pushbind(const string& topic, const string& pid, const string& endpt){
    bind[topic] = record_t(pid, endpt);
}
void DBv::pushconn(const string& topic, const string& pid, const string& endpt){
    conn[pid] = record_t(topic, endpt);
}
