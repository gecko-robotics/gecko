#include <iostream>
#include <variant>
#include <map>
#include <chrono>

using namespace std;

class Port {
public:
    Port(){}
    ~Port(){disc();}

    class Connect {
    public:
        Connect(){cout<<"connection!"<<endl;}
        std::chrono::system_clock::time_point time;
    };

    class Disconnect{
    public:
        Disconnect(){cout<<"disconnected ..."<<endl;}
    };

    // variant<initial, other, ...?
    variant<Disconnect, Connect> state;

    void conn(){state = Connect();}
    void disc(){state = Disconnect();}
};

template <class F1, class F2>
struct overload_set : F1, F2
{
    overload_set(F1 x1, F2 x2) : F1(x1), F2(x2) {}
    using F1::operator();
    using F2::operator();
};

template <class F1, class F2>
overload_set<F1,F2> overload(F1 x1, F2 x2)
{
    return overload_set<F1,F2>(x1,x2);
}

int main(){
    auto f = overload (
        []( int) { cout << __PRETTY_FUNCTION__ << endl; },
        [](char) { cout << __PRETTY_FUNCTION__ << endl; }
    );

    f('q');
    f( 2 );

    Port p;
    p.conn();
    cout << ">> " << p.state << endl;

    return 0;
}
