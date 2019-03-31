# :lizard: Gecko

This library uses:

- [ZeroMQ](https://zeromq.org) for sending an receiving messages, it uses:
    - Subscribe/publish
    - Request/reply
- [Msgpack](https://msgpack.org/index.html) for serialization of messages
- [zlib](https://www.zlib.net/) for compression of serialized messages (**TBD**)
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
- `Gecko`
    - `subscribe`: Finds publisher for subscribers
    - `advertise`: Broadcasts endpoint for topics
    - Periodically (~3 sec) broadcasts performance [TBD]
    - Interface to message logging [TBD]

# macOS

```
brew install msgpack
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
- currently use msgpack, looking at others like capnp
    - currently have to rewrite the message format for c++ *and* rewrite for
    python ... sucks
- https://conan.io/ ???

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

### Common (t suffix)

- vec_t: 3D vector
- quaternion_t: 4D quaternion
- twist_t:
- pose_t: robot orientation (quaternion) and postion (vector)
- wrench_t:

### Time Stamped Messages (st suffix)

- imu_st: IMU measurement (acceleration, angular velocity, magnetometer)
- lidar_st: lidar measurements, an array of (angle [deg], range [m])
