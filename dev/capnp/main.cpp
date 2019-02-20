#include "msg.capnp.h"
#include "imu.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize.h>
#include <capnp/serialize-packed.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(){
    capnp::MallocMessageBuilder message;
    IMU::Builder msg = message.initRoot<IMU>();
    msg.setTimestamp(0.567891011);
    // msg.initAccel();

    Vector::Builder v = msg.getAccel();
    v.setX(-1.2345);
    v.setY(-1.2345);

    // Vector::Builder v = msg.getGyro();
    v = msg.getGyro();
    v.setX(-1.2345);
    v.setY(-1.2345);

    // cout << (msg == msg) << endl; // doesn't work
    cout << msg.getAccel().getX() << endl;
    cout << msg.getGyro().getX() << endl;
    cout << msg.getGyro().getZ() << endl;
    cout << msg.getTimestamp()*1E6 << endl;
    cout << "word count: " << msg.totalSize().wordCount << endl;
    cout << "cap count: " << msg.totalSize().capCount << endl; // Size of the capability table

    auto b = capnp::messageToFlatArray(message);
    cout << "buff: " << b.asBytes().size() << endl;
    printf("hello\n");
    return 0;
}
