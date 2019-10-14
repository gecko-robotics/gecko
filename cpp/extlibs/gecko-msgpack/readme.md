# Gecko MsgaPack

This provides a serialization mechanism for `gecko` messages. It is not part of `gecko`
because I wanted to be modular and able to switch out to a different serialization
package if I needed to.

- [Msgpack](https://msgpack.org/index.html) for serialization of messages
- [cppzmq](https://github.com/zeromq/cppzmq) is a header only, high level zmq

## Install

The version of `msgpack` that comes with linux is old. Download and install a current version.

## macOS

```
brew install msgpack
```

## Messages

Message formats are pulled from ROS. There are both common messages and time
stamped messages.

### Common Message Types (\_t suffix)

- vec_t: 3D vector
- quaternion_t: 4D quaternion
- twist_t: linear(vec_t), angular(vec_t) rates
- pose_t: robot orientation (quaternion) and position (vector)
- wrench_t: force (vec_t), torque (vec_t)

### Time Stamped Messages (\_st suffix)

- imu_st: IMU measurement (acceleration, angular velocity, magnetometer)
- lidar_st: lidar measurements, an array of (angle [deg], range [m])

Should you need to change install location, you can change that during the make:
`cmake .. -DCMAKE_INSTALL_PREFIX=/home/kevin/.local`.

# Examples

The pub/sub examples below show how to use the `msgpack` adapter to
do a simple UDS pub/sub service.

## Subscriber

```cpp
#include <string>
#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>
#include <msgpack/msgpack_pub_sub.hpp>

using namespace std;

void sub()
{
    string endpt = zmqUDS("/tmp/0");
    Subscriber s;
    s.connect(endpt);

    MsgPack<imu_t> buffer;

    while (gecko::ok())
    {
        zmq::message_t msg = s.recv();
        imu_t m = buffer.unpack(msg);
        // do something with it ...
    }
}

int main(){
    sub();
    return 0;
}
```

## Publisher

```cpp
#include <string>
#include <iostream>
#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>
#include <msgpack/msgpack_pub_sub.hpp>

using namespace std;

void pub()
{
    string endpt = zmqUDS("/tmp/0");
    Publisher p;
    p.bind(endpt);
    Rate rate(1);

    MsgPack<imu_t> buffer;

    while (gecko::ok())
    {
        vec_t a(1,2,3);
        imu_t b(a,a,a);  // create new timestamp
        zmq::message_t msg = buffer.pack(b);

        p.pub(msg);

        cout << "pub" << endl;
        rate.sleep();
    }
}

int main(){
    pub();
    return 0;
}
```

# MIT License

**Copyright (c) 2019 Kevin J. Walchko**

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
