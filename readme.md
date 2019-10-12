# :lizard: Gecko

[![License](https://img.shields.io/pypi/l/pygecko.svg)](https://pypi.python.org/pypi/pygecko/)

## My robot software.

- Doesn't use [ROS](http://ros.org), ROS and ROS2 is a pain to install and maintain
on macOS and various linux systems
    - Uses some of the same ideas, constructs, architecture ideas, APIs but
    not strictly adhering to them
    - ROS tries to be too much
        - `catkin`/`colcon` are crap ... simple cmake is better
        - do not need to monkey with `env` and constantly source some setup file
- Uses [Zero MQ](http://http://zeromq.org/) as the inter-process communication
(uses both TCP and UDS) instead of RPC-XML or DDS
    - looked at Google's protobuf, but was more complex than I needed
    - using [`msgpack`](https://msgpack.org/index.html) to serialize data currently,
    but could be changed to something different  
    - instead of `ros2` use `gecko.py core` as the message hub
        - produces performance data (see below)
- `json` - config and launch files
- All of this runs on [Raspberry Pi 3](http://www.raspberrypi.org) with Raspbian
    - Also runs on macOS (UNIX)

# Architecture

![](docs/multiprocess-4.png)

- Programming languages
    - C++17
    - Python 3.7
- GeckoCore is a hub that tracks what computer publishes what topic and prints
node cpu/memory usage
    - Actually, when gecko processes start up, they tell geckocore their pid
    numbers so it can track usage using `psutil` library
    - Obviously this only works on processes located on the same machine as geckocore
    - GeckoCore really just displays info and keeps track of publisher topics/addresses
    - If a Pub/Sub is on a remote machine, then it's performance data is not displayed
    - There currently is no mechanism to get the remote performance data
- Inter process communications (IPC)
    - Publish/subscribe
    - Request/reply
    - Communications can be TCP or Unix Domain Sockets (UDS)
    - GeckoCore is optional, if you know the TCP/UDS address, you can tell the
    pub/sub or req/rep where the endpoint is. GeckoCore is just there to make
    things easier and provide performance info
- Serialization is done with `msgpack`
- Tools
    - bag: save data for replay later
    - topic: display pub/sub
    - service: display req/rep
    - launch: run multiple nodes at once
- Messages
    - std_msgs: vector, quaternion, twist, wrench, string
    - sensor_msgs: image, imu, lidar, joy
    - nav_msgs: path, odemetry
    - messages can be simple or timestamped
    - Python: messages are `namedtuples`

## Multicast Messages

Multicast is used to talk with `geckocore` and pass information between. The
advantage is you do not have to know the IP/Port address of a given core. You
use a `key` (ex: `dalek`, `local`, `bob`, `whatever`) that only that core will
respond too. You can thus make it robot centric, or whatever you want centric.

```
Connect: multiple nodes can connect to endpt
node -> core: key|topic|pid
core -> node: key|topic|endpt|ok

Bind: only one node can bind to an endpt
node -> core: key|topic|pid|endpt
core -> node: key|topic|endpt|ok
```

# Components

Gecko is written in a couple languages and uses common cross-language cross-platform
libraries to bring everything together. The primary 2 languages are C++ and
python 3.

| Component     | C++ | Python | Nodejs |
|---------------|-----|--------|--------|
| tools         |     | X      |        |
| IPC           | X   | X      |        |
| serialization | X   | X      |        |
| visualization |     |        |        |

## Tools

Like `ros2`, `gecko` is a bunch of tools written in python:

- `gecko core run|ping`
- `gecko multicast send|receive`
- `gecko bag play|record filename --loop`
    - bag will use the python library [the-collector](https://github.com/MomsFriendlyRobotCompany/the_collector)
    and store data using `pickle` so any python jupyter notebook or tools can
    use the data without gecko
- `gecko topic echo|bw|pub topic message`

## Linux Packaging with CMake

This will install the message formats into `gecko` in `/usr/local`.

```
cd gecko
mkdir build
cd build
cmake ..
make
make package
sudo dpkg -i gecko-x.x.x-linux.deb
```

# MIT License

**Copyright (c) 2010 Kevin J. Walchko**

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
