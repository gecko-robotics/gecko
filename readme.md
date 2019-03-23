# :lizard: Gecko

This library uses:

- [ZeroMQ](https://zeromq.org) for sending an receiving messages, it uses:
    - Subscribe/publish
    - Request/reply
- [Msgpack]() for serialization of messages
- [zlib]() for compression of serialized messages (**TBD**)
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
