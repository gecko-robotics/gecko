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
    - bash scripts for building, but might move to `zsh` because of macOS
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

# Transport IPC

Endpoint protocols:

- Transmisison Control Protocol (TCP): `tcp://host:port`
- Unix Domain Sockets (UDS): `ipc://path/to/file`

## Publisher and Subscriber

1. Start a GeckoCore instance
    1. The `key` defaults to host name, but can be set to any string
    1. The idea is every robot (computer) can have a core running
1. *Binder:* only 1 per port, can be either pub or sub
    1. Binders can use either TCP or UDS
    1. Binder opens a random port for data for TCP
    1. *Note:* either a publisher or a subscriber can bind to an endpoint
    1. A multicast message is sent to core
1. GeckoCore acknowledges the binder with an `ok` message
1. *Connecter:* can be many per endpoint, can be pub or sub
    1. First connector sends a multicast message to core asking for the endpoint
    for a topic
    1. Only 1 service can publish a topic and it exists on only 1 endpoint
1. GeckoCore:
    1. If topic is found, return the endpoint and an `ok` message
    1. If topic is *not* found, returns `None`

## Request and Reply

In work

## Core

This is the main message hub. GeckoCore also is passed the PIDs for processes on the
local machine and prints performance data on each process:

```bash
+========================================
| Processes Performance
| [24790] GeckoCore............. cpu:  0.3%  mem:  0.0%
| [24793] pub_ryan.............. cpu:  0.1%  mem:  0.0%
| [24795] pub_mike.............. cpu:  0.1%  mem:  0.0%
| [24796] sub_mike.............. cpu: 20.5%  mem:  0.0%
| [24797] pub_sammie............ cpu:  0.1%  mem:  0.0%
| [24798] sub_sammie............ cpu: 20.5%  mem:  0.0%
+------------------------------
| ESTABLISHED Connections
| pub_mike............ 192.168.86.22:50551 --> 192.168.86.22:50557
| sub_mike............ 192.168.86.22:50557 --> 192.168.86.22:50551
| pub_sammie.......... 192.168.86.22:50554 --> 192.168.86.22:50558
| sub_sammie.......... 192.168.86.22:50558 --> 192.168.86.22:50554
+------------------------------
| LISTEN Connections
| GeckoCore........... 192.168.86.22:11311
| pub_ryan............ 192.168.86.22:50548
| pub_mike............ 192.168.86.22:50551
| pub_sammie.......... 192.168.86.22:50554
+========================================
| Published Topics <topic>@tcp://<ip>:<port>
|  1: ryan@tcp://192.168.86.22:50548
|  2: mike@tcp://192.168.86.22:50551
|  3: sammie@tcp://192.168.86.22:50554
+========================================
```

```bash
========================================
 Geckocore [65975]
-------------
 Key: local
 Host IP: 10.0.1.57
 Listening on: 224.3.29.110:11311
-------------
Known Services [6]
 * hello:........................ tcp://10.0.1.57:65303
 * hey there:.................... tcp://10.0.1.57:65304
 * ryan:......................... tcp://10.0.1.57:65310
 * mike:......................... tcp://10.0.1.57:65311
 * sammie:....................... tcp://10.0.1.57:65312
 * scott:........................ tcp://10.0.1.57:65313

Binders [6]
 [65993] hello................. cpu:  0.0%  mem:  0.0%
 [65994] hey there............. cpu:  0.0%  mem:  0.0%
 [66008] ryan.................. cpu:  0.1%  mem:  0.0%
 [66010] mike.................. cpu:  0.1%  mem:  0.0%
 [66012] sammie................ cpu:  0.1%  mem:  0.0%
 [66014] scott................. cpu:  0.1%  mem:  0.0%

Connections [8]
 [65995] hello................. cpu: 20.7%  mem:  0.0%
 [65996] hello................. cpu: 20.9%  mem:  0.0%
 [65997] hey there............. cpu: 21.0%  mem:  0.0%
 [65998] hey there............. cpu: 20.8%  mem:  0.0%
 [66011] mike.................. cpu: 19.0%  mem:  0.0%
 [66013] sammie................ cpu: 19.0%  mem:  0.0%
 [66015] scott................. cpu: 19.4%  mem:  0.0%
 [66009] ryan.................. cpu: 18.7%  mem:  0.0%
```

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
- `gecko multicast a|b|c`
    - send multicast messages (just simple a, b, or c) between computers to
    test connectivity 
- `gecko bag play|record filename --loop`
    - bag will use the python library [the-collector](https://github.com/MomsFriendlyRobotCompany/the_collector)
    and store data using `pickle` so any python jupyter notebook or tools can
    use the data without gecko
- `gecko topic echo|bw|pub topic message`


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
