# :lizard: Gecko

This library uses:

- [ZeroMQ](https://zeromq.org) for sending an receiving messages, it uses:
    - Subscribe/publish
    - Request/reply
- C++17 syntax

## ROS Influence

This draws many ideas from [ros](https://ros.org), but doesn't strickly always
follow the API. It also pulls a lot from rospy. This library is step as:

- The gecko library is serialization independent. I have chosen `msgpack`, but
you could swap it out for Google's `protobuf`.
    - All `zmq` interfaces are standarized on `zmq::message_t` as input/output
    so you can change the serialization easily
    - Just pick your library of choice and write classes/functions that
    transpose your favorite message format to/from `zmq::message_t` and you
    should be good
- `gecko` can use either unix domain sockets (UDS) or TCP sockets for messages
- `zmq` publisher only sends one type of message (e.g., `imu_t`) per topic
    - *For example:* topic `robot_pose` will only send `pose_t` on that topic
- When a message is published on a topic, the topic name is not published too.
This is because, *really*, topics are deconflicted by port number and not
something found *in* the message sent


# macOS

```
brew install zeromq
```

# Linux (Ubuntu)

The `apt` package is old, so to use the current one and compile
it for C++17 and install it to `~/.local` (to not polute my system
libraries), I do:

```
git clone https://github.com/msgpack/msgpack-c.git
cd msgpack-c
mkdir build
cd build
cmake -DMSGPACK_CXX[17]=ON -DCMAKE_INSTALL_PREFIX=/home/kevin/.local ..
make install
```

# Status

- multicast works (sort of)
    - right now my core is written python ... i want to convert it c++ eventually
- pub/sub and req/rep working
- need to sort out `Node` and `gecko` classes, they sort of do the same thing

## Multicast Messages

Multicast is used to talk with `geckocore` and pass information between. The
advantage is you do not have to know the IP/Port address of a given core. You
use a `key` (ex: `dalek`, `local`, `bob`, `whatever`) that only that core will
respond too. You can thus make it robot centric, or whatever you want centric.

```
sub: key|topic|pid
=> sub: key|topic|endpt

pub: key|topic|pid|endpt
=> pub: key|topic|endpt|ok

---
multiple nodes can connect to endpt
connect: key|topic|pid
=> connect: key|topic|endpt

only one node can bind to an endpt
bind: key|topic|pid|endpt
=> bind: key|topic|endpt|ok
```

# Gecko C++ API

## `gecko`

Many of these ideas are pulled from ROS

- `gecko::init()` setups the system
- `gecko::Rate(hz)` returns a rate object
- returns publisher/subscriber (see variations below)
- `gecko::is_shuttingdown()` or `gecko::ok()` returns the process status (true/false)
- `gecko::log(level, string)` prints log info

## Publisher and Subscriber

Ros has `advertise` and `subscribe` functions, `zmq` provides a little more flexibility
so `gecko` has deviated from that:

- `gecko::pubBindTCP(key, topic)`: returns a publisher that binds to local system
and informs `geckocore` (using key) of the the topic it publishes from ip/port
- `gecko::subConnectTCP(key, topic)`: returns a subscriber that talks to `geckocore`
to get the ip/port publishing the topic
- `gecko::pubBindUDS(key, topic)`: variation
- `gecko::pubConnectTCP(key, topic)`: variation
- `gecko::pubConnectUDS(key, topic)`: variation
- `gecko::subBindUDS(key, topic)`: variation
- `gecko::subBindTCP(key, topic)`: variation
- `gecko::subConnectUDS(key, topic)`: variation

With `zmq`, a publisher and subscriber can both bind or connect to a TCP or
a UDS endpoint.

## Messages

Message formats are pulled from ROS. There are both common messages and time
stamped messages.

### Common Message Types (\_t suffix)

- vec_t: 3D vector
- quaternion_t: 4D quaternion
- twist_t:
- pose_t: robot orientation (quaternion) and postion (vector)
- wrench_t:

### Time Stamped Messages (\_st suffix)

- imu_st: IMU measurement (acceleration, angular velocity, magnetometer)
- lidar_st: lidar measurements, an array of (angle [deg], range [m])

# Examples

The examples below create a simple pub/sub service. Note, they use `msgpack` for
data serialization. That is not part of this library. Please build/install `gecko-msgpack`
to use this.

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

## Performance

`geckocore` has a thread that prints out performance information of all
processes that have talked to it.

```
========================================
 Geckocore [96103]
-------------
 Start: 09-06-2019 09:03:49
 Key: local
 Host IP: 10.0.1.70
 Listening on: 224.3.29.110:11311
 CPU: 0.0   Memory: 0.0
--------------------
 Binder [4]
 Topic                           CPU  MEM  EndPt
 > mike[97311]...................0.0  0.0  tcp://10.0.1.70:51175
 > ryan[97305]...................0.0  0.0  tcp://10.0.1.70:51174
 > sammie[97315].................0.0  0.0  tcp://10.0.1.70:51176
 > scott[97320]..................0.0  0.0  tcp://10.0.1.70:51177
--------------------
 Connector [8]
 Topic                           CPU  MEM  EndPt
 > ryan[97307]...................20.5 0.0  tcp://10.0.1.70:51174
 > ryan[97308]...................22.2 0.0  tcp://10.0.1.70:51174
 > mike[97312]...................20.5 0.0  tcp://10.0.1.70:51175
 > mike[97313]...................16.5 0.0  tcp://10.0.1.70:51175
 > sammie[97316].................23.7 0.0  tcp://10.0.1.70:51176
 > sammie[97318].................22.0 0.0  tcp://10.0.1.70:51176
 > scott[97321]..................25.8 0.0  tcp://10.0.1.70:51177
 > scott[97323]..................25.9 0.0  tcp://10.0.1.70:51177
 ```

# ToDo

- [ ] Add json file support
- [ ] Make python bindings and maybe get rid of separate pygecko?
- [ ] Windozes support ... ha, ha, ha, ha ... probably not :P
- [ ] Log server
- [ ] echo
- [ ] Bag files ... probably embed in message serialization library
- [ ] Add submodules for: serialization, zmq
- [ ] Better way to add new message types
- [ ] serialize opencv images
- [ ] Command line args that works both on macOS and linux
- [ ] travis-ci setup

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
