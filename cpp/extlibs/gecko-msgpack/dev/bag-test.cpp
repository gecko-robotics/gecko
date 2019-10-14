// #include <msgpack.hpp>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <gecko/msgpack/msgs.hpp>
#include <map>
#include <any>

using namespace std;


class BagIt {
public:
    BagIt(){}

    template<class MSG>
    void push(string key, const MSG& m){
        if (db.count(key) > 0){
            db[key].push_back(m);
        }
        else {
            db[key].push_back(m);
        }
    }

    void dump(std::string filename){
        
    }

// protected:
    std::map<std::string, std::vector<std::any>> db;  // issues w/std::any?
};

int main(){
    BagIt bag;

    for (int i=0; i<10; i++) {
        vec_t a(double(i)/10,200,-i);
        bag.push<vec_t>("hi", a);

        quaternion_t q(1,2,3,4);
        bag.push<quaternion_t>("ho", q);
    }

    for (const auto& [k,v]: bag.db) {
        cout << k << endl;
        if (k == "hi") for (const auto& m: v) cout << any_cast<vec_t>(m) <<endl;
        else for (const auto& m: v) cout << any_cast<quaternion_t>(m) <<endl;
    }

    return 0;
}
