#include "directory.hpp"
#include <stdio.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////

void Directory::push(const string& key, const string& topic, const string& endpt){
    db[key][topic] = endpt;
}

void Directory::pop(const string& key, const string& topic){
    printf("*** FIXME ***\n");
}

string Directory::find(const string& key, const string& topic){
    string ret;

    auto dir = db.find(key);
    if(dir != db.end()){
        auto row = dir->second.find(topic);
        if(row != dir->second.end()) ret = row->second;
    }

    return ret; // empty if not found
}

void Directory::print() const {
    printf("====================================\n");
    printf(" Directory keys: %d\n", numberKeys());
    for (auto const& [key,dir]: db){
        printf("[%s: %d]------------------\n", key.c_str(), numberTopics(key));
        for (auto [topic,endpt]: dir){
            printf(" %s => %s\n", topic.c_str(), endpt.c_str());
        }
    }
    printf("-----------------------\n");
}

int Directory::numberKeys() const {
    return db.size();
}

int Directory::numberTopics(const string& key) const {
    auto dir = db.find(key);
    if(dir != db.end()) return dir->second.size();
    return -1; // key wasn't found
}
