#include <gecko/msgpack/serialization.hpp>
#include <string>
#include <iostream>

using namespace std;

class a_t: public base_t {
public:
    a_t(): base_t(31) {}
    a_t(double a, double b): base_t(31), illuminance(a), variance(b) {}
    a_t(const a_t& a): base_t(31), illuminance(a.illuminance), variance(a.variance) {}
    bool operator==(const a_t& v) const {
        if((illuminance == v.illuminance) && (variance == v.variance))
            return true;
        return false;
    }
    double illuminance;     // Measurement of the Photometric Illuminance in Lux.
    double variance;        // 0 is interpreted as variance unknown

    void print() const {
        // printf(" [%f %f %f %f]\n",illuminance,variance);
        cout << "[" << illuminance << ", " << variance << "]" << endl;
    }
    MSGPACK_DEFINE(illuminance, variance);
};

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
    double illuminance;     // Measurement of the Photometric Illuminance in Lux.
    double variance;        // 0 is interpreted as variance unknown

    void print() const {
        // printf(" [%f %f %f %f]\n",illuminance,variance);
        cout << "[" << illuminance << ", " << variance << "]" << endl;
    }
    MSGPACK_DEFINE(illuminance, variance);
};

int main(){
    a_t a(1,2);

    MsgPack<a_t> packer;
    zmq::message_t msg = packer.pack(a);
    a_t b = packer.unpack(msg);

    a.print();
    b.print();

    cout << (a == b) << endl;

    // --------------------------------------
    b_t c(3,4);

    MsgPack<b_t> packer2;
    zmq::message_t msg2 = packer2.pack(c);
    b_t d = packer2.unpack(msg2);

    c.print();
    d.print();
    cout << (c == d) << endl;

    return 0;
}
