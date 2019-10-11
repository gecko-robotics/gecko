#include <gecko/msgpack/serialization.hpp>
#include <string>
#include <iostream>
#include <gecko/msgpack/msgs.hpp>

using namespace std;


/*
So here is a new message I want to create.
*/
class b_t: public base_t {
public:
    b_t(): base_t(31) {}
    b_t(double a, double b): base_t(31), illuminance(a), variance(b) {}
    b_t(const b_t& a): base_t(31), illuminance(a.illuminance), variance(a.variance) {}
    bool operator==(const b_t& v) const {
        if((illuminance == v.illuminance) && (variance == v.variance))
            return true;
        return false;
    }
    double illuminance; // Measurement of the Photometric Illuminance in Lux.
    double variance;    // 0 is interpreted as variance unknown

    void print() const {
        cout << "[" << illuminance << ", " << variance << "]" << endl;
    }
    MSGPACK_DEFINE(illuminance, variance);
    GECKO_MSG(b_t, 31);
};

int main(){
    b_t c(3,4);
    zmq::message_t msg2 = c.pack();
    b_t d(msg2);

    c.print();
    d.print();
    cout << (c == d) << endl;

    return 0;
}
