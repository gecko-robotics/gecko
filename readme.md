# :lizard: Gecko

[![License](https://img.shields.io/pypi/l/pygecko.svg)](https://pypi.python.org/pypi/pygecko/)

## My robot software.

- Doesn't use [ROS](http://ros.org), ROS and ROS2 is a pain to install and maintain
on macOS and various linux systems
    - Uses some of the same ideas, constructs, architecture ideas, APIs but
    not strictly adhering to them
    - ROS tries to be too much
        - `catkin`/`colcon` are crap ... simple cmake is better
        - do not need to monkey with `env`
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

- GeckoCore is a hub that tracks what computer publishes what topic and prints
node cpu/memory usage
    - Actually, when gecko processes start up, they tell geckocore their pid
    numbers so it can track usage using `psutil` library
    - Obviously this only works on processes located on the same machine as geckocore
    - GeckoCore really just displays info and keeps track of publisher topics/addresses
- Any number of pubs can talk to any number of sub ... it is not a one-to-one relationship
- Pubs/Subs can exist on remote machines
    - If a Pub/Sub is on a remote machine, then it's performance data is not displayed
    - There currently is no mechanism to get the remote performance data


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

| Component | C++    | Python | Nodejs |
|-----------|--------|--------|--------|
| tools     |        | gecko  |        |
| IPC       | cppzmq | zmq    |        |
| serialization | cmsgpack | cppzmq |  |
| visualization |    |        | gviz   |

## Tools

Like `ros2`, `gecko` is a bunch of tools written in python:

- `gecko core run|ping`
- `gecko multicast send|receive`
- `gecko bag play|record filename --loop`
- `gecko topic echo|bw|pub topic message`
