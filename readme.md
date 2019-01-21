# :lizard: Gecko

This library uses:

- [ZeroMQ](https://zeromq.org) for sending an receiving messages, it uses:
    - Subscribe/publish
    - Request/reply
- [Msgpack]() for serialization of messages
- [zlib]() for compression of serialized messages
- C++17 syntax

## ROS Influence

This draws many ideas from [ros](https://ros.org), but doesn't strickly always
follow the API. It also pulls a lot from rospy. This library is step as:

- There is no `roscore`, but rather publishers are found via simple multicast
message calls
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
