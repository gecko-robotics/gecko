#!/usr/bin/env python3

import threading
import time
from mbeacon import BeaconServer, BeaconFinder
from transport import Ascii
# from collections import namedtuple
# from enum import Enum, IntFlag


# Pub_t = namedtuple('Pub_t', 'key topic pid funcname')
# Sub_t = namedtuple('Sub_t', 'key topic pid funcname')
# Perf_t = namedtuple('Perf_t', 'key nummsg msgrate datarate funcname')
# Connect_t = namedtuple('Connect_t', 'topic ip')

# MsgsIPC = Enum('MsgsIPC', 'Pub Sub Resp Perf')
# TTL = IntFlag('TTL',{'host': 0, 'subnet': 1, 'site': 31})
#
# class IPC(object):
#     def __init__(self, key, kind, topic, pid, funcname):
#         self.msg = (key, kind, topic, pid, funcname,)
#
# class PubIPC(IPC):
#     def __init__(self, key, topic, pid, funcname):
#         IPC.__init__(self, key, MsgsIPC.Pub, topic, pid, funcname,)
#
# class SubIPC(IPC):
#     def __init__(self, key, topic, pid, funcname):
#         IPC.__init__(self, key, MsgsIPC.Sub, topic, pid, funcname,)
#
# class PerfIPC(object):
#     def __init__(self, key, nummsg, msgrate, datarate, funcname):
#         self.msg = (key, MsgsIPC.Perf, nummsg, msgrate, datarate, funcname,)
#
# class CoreIPC(object):
#     def __init__(self, topic, tcpaddr):
#         self.msg = (topic, tcpaddr,)


services = {}

def callback(data, address):
    print(">> Address: {}".format(address))
    print(">> Data: {}".format(data))

    # if data[1] == MsgsIPC.Pub:
    #     services[data[2]] = address
    #     ret = ('pub register', data[2],)
    # elif (data[1] == MsgsIPC.Sub) and (data[2] in services.keys()):
    #     ret = ('sub request', data[2], services[data[2]],)
    # elif data[1] == MsgsIPC.Perf:
    #     ret = (1234,)
    #     print('>> performance data')
    # else:
    #     ret = ('unknown',)
    # return ret
    return ('local',"1234",)

def server(e):
    # print(TTL.host == 0)
    bs = BeaconServer('local', callback=callback,handler=Ascii)
    bs.start()

    while e.isSet():
        time.sleep(.1)

def client(e):
    bf = BeaconFinder('local',handler=Ascii)

    time.sleep(1)

    # publisher setup
    ans = None
    while ans is None:
        # msg = PubIPC('local','bob',12345,'fun').msg
        msg = ('local',"hi",)
        ans = bf.send(msg)
    print(ans)

    ans = None
    while ans is None:
        # msg = PubIPC('local','tom',12345,'fun').msg
        msg = ('local',"ho",)
        ans = bf.send(msg)
    print(ans)

    while e.isSet():
        time.sleep(1)
    #     print('-'*40)
    #
    #     msg = SubIPC('local','tom',12345,'fun').msg
    #     ret = bf.send(msg)
    #     print('>> {}'.format(ret))
    #
    #     msg = SubIPC('local','bob',12345,'fun').msg
    #     ret = bf.send(msg)
    #     print('>> {}'.format(ret))
    #
    #     msg = PerfIPC('local',1,2,3,'fun').msg
    #     bf.send(msg)


if __name__ == "__main__":
    e = threading.Event()
    e.set()

    s = threading.Thread(target=server, args=(e,))
    s.start()

    c = threading.Thread(target=client, args=(e,))
    c.start()

    time.sleep(6)
    e.clear()
