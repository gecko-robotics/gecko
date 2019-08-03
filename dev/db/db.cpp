#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <stdexcept>

using namespace std;

template<class KEY, class VALUE>
class DB {
public:
    DB(){}
    // DB(std::initializer_list)
    DB(const std::map<KEY,VALUE>& m): db(m) {}
    DB(const std::map<KEY,VALUE>&& m): db(std::move(m)) {}
    VALUE& get(const KEY& k){
        typename std::map<KEY, VALUE>::iterator it;
        it = db.find(k);
        if (it == db.end()) printf("error: %d\n", k);
        return it->second;
    }
    inline void push(const KEY& key, const VALUE& value){
        db[key] = value;
    }
    inline void pop(const KEY& key){
        db.erase(key);
    }
    // void print(){
    //     if (db.size() == 0) return;
    //     for (auto const& [key, val] : db) {}
    // }
    VALUE& operator[](const KEY& key){return get(key);}
    VALUE& operator[](KEY&& key) { return get(std::move(key)); }
    typename map<KEY,VALUE>::iterator begin(){return db.begin();}
    typename map<KEY,VALUE>::iterator end(){return db.end();}
    typename map<KEY,VALUE>::const_iterator begin() const {return db.begin();}
    typename map<KEY,VALUE>::const_iterator end() const {return db.end();}

    // void saveJson(const std::string& filename){throw NotImplemented("DBv2::save()");} // value?
    // void loadJson(const std::string& filename){throw NotImplemented("DBv2::load()");} // value?

protected:
    std::map<KEY, VALUE> db;
};

// inline
int find(const int& key, const map<int,int>& db) {
    if (std::map<int, int>::iterator it; it == db.end())
        throw runtime_error("find error");
    else
        return it->second;
}


// int find(const int& k, const map<int,int>& db){
//     std::map<int, int>::iterator it;
//     it = db.find(k);
//     // if (it == db.end()) {
//     if (it == db.end()) {
//         // cout << "> " << k << endl;
//         // throw exception();
//         throw runtime_error("find error");
//     }
//     return it->second;
// }

int main(){
    // DB<int,int> db;
    // db.push(3,4);
    //
    // cout << db[3] << endl;
    // // db[33] = 22;
    // cout << db[3] << endl;
    //
    // DB<char,float> db2{{{'a',1.2},{'b',3.4},{'c',3.4}}};
    // cout << db2['b'] << endl;
    //
    // for (const auto& [k,v]: db2){
    //     cout << "> " << k << " " << v << endl;
    // }
    //
    // typedef struct {int a;} Test;
    // DB<int, Test> db3;
    // db3[2] = Test{3};
    // cout << db3[2].a << endl;

    map<int,int> dd{{1,11},{2,22},{3,33}};
    dd.insert({4, 44});
    try {
        cout << dd[1] << endl;
        cout << dd[4] << endl;
        // cout << dd.size() << " " << dd.max_size() << endl;
        cout << find(3, dd) << endl;
        cout << dd.find(3)->second << endl;
    }
    catch (const exception& e){
        cout << "error dd: " << e.what() << endl;
    }

    return 0;
}
