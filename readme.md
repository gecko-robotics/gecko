# Gecko

## My robot software.

- Doesn't use [ROS](http://ros.org), ROS is a pain to install and maintain
on macOS and various linux systems
    - Uses some of the same ideas, constructs, architecture ideas, APIs but
    not strictly adhering to them
- Uses [Zero MQ](http://http://zeromq.org/) as the inter-process communication
(uses both TCP and UDS) instead of RPC-XML
    - looked at Google's protobuf, but was more complex than I needed
    - using [`msgpack`](https://msgpack.org/index.html) to serialize data currently,
    but could be changed to something different  
    - instead of `roscore` use `geckocore.py` as the message hub
        - produces performance data (see below)
    - instead of `roslaunch` use `geckolaunch.py`
- `json` - config and launch files
- All of this runs on [Raspberry Pi 3](http://www.raspberrypi.org)
    - Also runs on macOS (UNIX)

# Architecture

![](pics/multiprocess-2.png)

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
multiple nodes can connect to endpt
connect: key|topic|pid
=> conn: key|topic|endpt|ok

only one node can bind to an endpt
bind: key|topic|pid|endpt
=> bind: key|topic|endpt|ok
```
