#include <gtest/gtest.h> // googletest header file
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

TEST(msgpack, new_msg){
    b_t c(3,4);
    ASSERT_NO_THROW(c.pack());
    zmq::message_t msg = c.pack();
    b_t d(msg);
    ASSERT_NO_THROW(b_t dummy(msg));
    ASSERT_TRUE(d == c);
    ASSERT_EQ(d,c);
}
