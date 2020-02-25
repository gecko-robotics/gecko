# :lizard: Gecko C++

![Cpp](https://github.com/gecko-robotics/gecko/workflows/Cpp/badge.svg)

My robotic framework

- [ZeroMQ](https://zeromq.org)
    - [cppzmq](https://github.com/zeromq/cppzmq)
- C++17 syntax
    - Apple LLVM version 10.0.1 (clang-1001.0.46.4)
    - c++ (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
- [json](https://github.com/nlohmann/json)
- [Google Test](https://github.com/google/googletest)
- Publish/subscriber and request/reply architecture
- Multicast to find nodes

# Organization

Gecko libraries are stored in stored in `/opt/gecko` to keep them separated from
system libraries:

```
/opt/gecko
     +- bin/
     +- include/  # gecko dependencies
     +- lib/      # gecko dependencies
     +- <gecko_version>  # core gecko includes/libraries/drivers
     |   +- bin/
     |   +- include/
     |   +- lib/
     |   |   +- cmake/   # gecko cmake
     |   +- modules/  # drivers or other sw bound to a gecko version
     |       +- protobuf/  # example serialization protocal would be here
     |       +- <driver_a>   # these are modular and simple rm to remove
     |           +- bin/
     |           +- include/
     |           +- lib/
     |           +- config/  # any sort of configuration files or cmake
     +- share/
         +- cmake/  # gecko dependency cmake
```

The goal of this organization is to keep things modular. Different versions
of gecko can exist and different modules can exist. At any time, you can delete
a gecko version or module and not break everything.

# Building

To build locally, do the following to get this repo and set things up
for needed libraries:

```
git clone https://github.com/gecko-robotics/gecko.git
cd gecko/cpp
./build_extlibs.sh
mkdir build
cd build
cmake ..
make install
```

# Examples

See the `examples` folder.

# Gecko C++ API

## `gecko`

Many of these ideas are pulled from ROS

- `gecko::init()` setups the system
- `gecko::Rate(hz)` returns a rate object
- `gecko::is_shuttingdown()` or `gecko::ok()` returns the process status (true/false)
- `gecko::log(level, string)` prints log info

## Publisher and Subscriber

Ros has `advertise` and `subscribe` functions, `zmq` provides a little more
flexibility than ROS, so `gecko` has deviated from that:

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
a UDS endpoint. Publishers do not *always* bind and subscribers do not *always*
connect.

# Request and Reply

Unlike pub/sub where either can bind or connect, the *reply only binds* and the
*request only connects*:

- `gecko::repBindTCP(key, topic)`: returns a reply node that binds to local system
- `gecko::repBindUDS(key, topic)`: variation
- `gecko::reqConnectTCP(key, topic)`: returns a  request node that connects to a reply
- `gecko::reqConnectUDS(key, topic)`: variation

# Examples

The examples below create a simple pub/sub service. The message
format defaults to a `zmq::message_t` which is a binary string. You
change this to:

- msgpack (gecko-msgpack)
- protobuf (gecko-msgpack)

## Subscriber

```cpp
#include <string>
#include <gecko/gecko.hpp>

using namespace std;

void sub()
{
    string endpt = zmqUDS("/tmp/0");
    Subscriber s;
    s.connect(endpt);

    while (gecko::ok())
    {
        zmq::message_t msg = s.recv();
        string s(msg.data(), msg.size())
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

using namespace std;

void pub()
{
    string endpt = zmqUDS("/tmp/0");
    Publisher p;
    p.bind(endpt);
    Rate rate(1);

    while (gecko::ok())
    {
        string s{"hello there"};
        zmq::message_t msg(s.data(), s.size());

        p.publish(msg);

        cout << "pub" << endl;
        rate.sleep();
    }
}

int main(){
    pub();
    return 0;
}
```

# ToDo

- [x] Add json support [nlohmann-json](https://github.com/nlohmann/json) for c++
- [ ] Look at yaml library: https://github.com/jbeder/yaml-cpp/
- [ ] Setup request/reply node
- [x] break `marko` out of `extlibs` folder into its own library
- [x] Investigate Conan.io as a way to organize gecko and dependancies
    - Conan.io difficult to setup and make work. I eventually could build
    packages and get them put on a local server I could run, but gecko had
    issues using the packages to build itself. After several days of trying to
    get it to work, I gave up.

# MIT License

**Copyright (c) 2014 Kevin J. Walchko**

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
