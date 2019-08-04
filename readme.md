# :lizard: Gecko

My robotic framework

- [ZeroMQ](https://zeromq.org)
    - [cppzmq](https://github.com/zeromq/cppzmq)
- C++17 syntax
    - Apple LLVM version 10.0.1 (clang-1001.0.46.4)
    - c++ (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
- [msgpack](https://msgpack.org/index.html)
- [json](https://github.com/nlohmann/json)
- [Google Test](https://github.com/google/googletest)
- Publish/subscriber and request/reply architecture
- Multicast to find nodes

## Building

```
git clone --recursive https://github.com/gecko-robotics/gecko.git
git submodule update --init --recursive
cd gecko
mkdir build
cd build
cmake ..
```

## ZMQ

### cppzmq

I use a high level C++ abstractin of the lower level zmq to build gecko.
This contains a submodule of the git repo for cppzmq and requires the following
command to set it up:

### macOS

```
brew install zeromq
```

### Linux (Ubuntu)

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

### libzmq

```
sudo apt install libzmq3-dev
```

### boost

I generally hate boost because it is a pain to work with. Right now you don't
need it.

```
sudo apt install libboost-dev
```


# Gecko C++ API

## Multicast Messages

Multicast is used to talk with `geckocore` and pass information between. The
advantage is you do not have to know the IP/Port address of a given core. You
use a `key` (ex: `dalek`, `local`, `bob`, `whatever`) that only that core will
respond too. You can thus make it robot centric, or whatever you want centric.

```
multiple nodes can connect to endpt
connect: key|topic|pid
=> conn: key|topic|endpt|ok

only one node can bind to an endpt
bind: key|topic|pid|endpt
=> bind: key|topic|endpt|ok
```

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
a UDS endpoint. Publishers do not *always* bind and subscribers do not *always*
connect.

# Examples

The examples below create a simple pub/sub service. Note, they use `msgpack` for
data serialization. That is not part of this library. Please build/install `gecko-msgpack`
to use this.

## Subscriber

```cpp
#include <string>
#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>

using namespace std;

void sub()
{
    string endpt = zmqUDS("/tmp/0");
    Subscriber s;
    s.connect(endpt);

    while (gecko::ok())
    {
        zmq::message_t msg = s.recv();
        imu_t m(msg);
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

using namespace std;

void pub()
{
    string endpt = zmqUDS("/tmp/0");
    Publisher p;
    p.bind(endpt);
    Rate rate(1);

    while (gecko::ok())
    {
        vec_t a(1,2,3);
        imu_t b(a,a,a);  // create new timestamp
        zmq::message_t msg = b.pack();

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

There are 4 publishers and 8 subscribers (2 per topic):

```
(cv) kevin@Logan tcp $ ./tcp.py
----------------------------------
GeckoPy
-----------
  Process: pub_ryan
  PID: 97305
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_ryan
  PID: 97307
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_ryan_2
  PID: 97308
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: pub_mike
  PID: 97311
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_mike
  PID: 97312
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_mike_2
  PID: 97313
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: pub_sammie
  PID: 97315
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_sammie
  PID: 97316
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_sammie_2
  PID: 97318
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: pub_scott
  PID: 97320
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_scott
  PID: 97321
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_scott_2
  PID: 97323
  Host: 10.0.1.70
----------------------------------
>> bind raw: ['local', 'ryan', '97305', 'ok']
>> pub_ryan:[0] published msg
>> conn raw:  ['']
>> conn raw:  ['local', 'ryan', 'tcp://10.0.1.70:51174', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'ryan', 'tcp://10.0.1.70:51174', 'ok']
[>] Receiving messages on ALL topics...
>> bind raw: ['local', 'mike', '97311', 'ok']
>> pub_mike:[0] published msg
>> conn raw:  ['local', 'mike', 'tcp://10.0.1.70:51175', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'mike', 'tcp://10.0.1.70:51175', 'ok']
[>] Receiving messages on ALL topics...
>> bind raw: ['local', 'sammie', '97315', 'ok']
>> pub_sammie:[0] published msg
>> conn raw:  ['']
>> conn raw:  ['local', 'sammie', 'tcp://10.0.1.70:51176', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'sammie', 'tcp://10.0.1.70:51176', 'ok']
[>] Receiving messages on ALL topics...
>> bind raw: ['local', 'scott', '97320', 'ok']
>> pub_scott:[0] published msg
>> conn raw:  ['local', 'scott', 'tcp://10.0.1.70:51177', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'scott', 'tcp://10.0.1.70:51177', 'ok']
[>] Receiving messages on ALL topics...
>> pub_ryan:[1] published msg
>> sub_ryan:ryan: {'time': 0.4579482078552246}
>> sub_ryan_2:ryan: {'time': 0.4579482078552246}
>> pub_mike:[1] published msg
>> sub_mike:mike: {'time': 0.4434690475463867}

... more messages streaming by

>> sub_sammie:sammie: {'time': 6.8596580028533936}
>> sub_sammie:sammie: {'time': 6.8596580028533936}
>> sub_scott:scott: {'time': 6.861537933349609}
>> sub_scott_:scott: {'time': 6.861537933349609}
>> sub_ryan:ryan: {'time': 6.969848155975342}
>> pub_ryan:[15] published msg
>> pub_mike:[15] published msg
>> sub_ryan_2:ryan: {'time': 7.471033096313477}
>> sub_mike:mike: {'time': 7.450994253158569}
>> sub_mike_2:mike: {'time': 7.450994253158569}
>> pub_sammie:[15] published msg
>> pub_scott:[15] published msg
>> sub_sammie:sammie: {'time': 7.360673904418945}
>> sub_sammie:sammie: {'time': 7.360673904418945}
>> sub_scott:scott: {'time': 7.3624796867370605}
>> sub_scott_:scott: {'time': 7.3624796867370605}
^Cmain process got ctrl-c
sub bye ...
pub bye ...
sub bye ...
pub bye ...
sub bye ...
sub bye ...
pub bye ...
pub bye ...
sub bye ...
sub bye ...
sub bye ...
sub bye ...
```

# ToDo

- [ ] Make python bindings and maybe get rid of separate pygecko?
- [ ] Windozes support ... ha, ha, ha, ha ... probably not :P
- [ ] Log server
- [ ] `geckotopic` echo/pub/list/find tools
- [ ] Learn anything from the bluezero service
- [ ] Bag files with geckobag ... probably embed in message serialization library
- [x] Add submodule for gecko-msgpack
- [x] Add submodule for cppzmq
- [ ] Look at yaml library: https://github.com/jbeder/yaml-cpp/
- [x] Add submodule for [nlohmann-json](https://github.com/nlohmann/json)
- [ ] Add json file setup
- [ ] Add json file `geckolaunch`
- [x] Better way to add new message types
- [x] serialize opencv images
- [ ] Local nodes only, show performance ... or figure how to handle remote nodes
- [ ] Remote nodes, investigate use heartbeat w/watchdog timer to determine if alive (node -> core)
- [ ] Remote nodes, investigate ping, return performance info (core -> node)
- [ ] Ping tool: all geckocores respond with info about themselves

Robot stuff
- [ ] visual odom https://github.com/MichaelGrupp/evo  https://michaelgrupp.github.io/evo/
- [ ] AI vision using http://cocodataset.org/#home
- [ ] AI https://skymind.ai/wiki/convolutional-network
- [ ] https://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets
- [ ] Look into NN with https://keras.io/
- [ ] Does https://openai.com/ help with anything?

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
