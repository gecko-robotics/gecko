# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
from pygecko.transport.zmq_sub_pub import Pub, Sub
from pygecko.transport.zmq_base import zmqTCP
from pygecko.transport.zmq_base import zmqUDS
import time
import multiprocessing as mp
from .mc_core_socket import BeaconFinder


def Binder(key, topic, Conn, fname=None, queue_size=5):
    """
    Creates a publisher that can either connect or bind to an address.

    bind -> (key, topic, pid, endpt)
    bind <- (key, topic, pid, ok)

    key: geckocore key
    topic: pub/sub topic name
    Conn: either Pub or Sub
    fname: file path for UDS
    queue_size: how many messages to queue up, default is 5
    """
    global g_geckopy
    p = Conn()
    p.topics = topic  # need to keep track
    # if (addr is None) and (bind):
    # addr = g_geckopy.proc_ip
    # addr = Proto(addr)
    # print('>> pub', addr)
    # addr = g_geckopy.core_inaddr

    # if bind:
    # port = p.bind(addr, queue_size=queue_size, random=True)
    # g_geckopy.register_publisher(topics, port)
    bf = BeaconFinder(key)
    pid = mp.current_process().pid

    if fname:
        endpt = zmqUDS(fname)
        p.bind(endpt, queue_size=queue_size)
        msg = (key, topic, str(pid), endpt)

        # print(p)
        # print(msg)
    else:
        # addr = g_geckopy.proc_ip
        addr = zmqTCP(g_geckopy.proc_ip)
        port = p.bind(addr, queue_size=queue_size, random=True)
        endpt = zmqTCP(g_geckopy.proc_ip, port)
        msg = (key, topic, str(pid), endpt)

    retry = 5

    for i in range(retry):
        data = bf.send(msg)
        # print(">> bind raw:", data)

        if data is None:
            time.sleep(0.5)
            continue

        if (len(data) == 4) and (data[0] == key) and (data[1] == topic) and (data[3] == "ok"):
            # print("geckopy.Binder SUCCESS", i)
            g_geckopy.binders[topic] = endpt
            return p

    return None


def pubBinderTCP(key, topic, queue_size=5):
    return Binder(key, topic, Pub, queue_size=queue_size)


def pubBinderUDS(key, topic, fname, queue_size=5):
    return Binder(key, topic, Pub, fname=fname, queue_size=queue_size)


def subBinderTCP(key, topic, queue_size=5):
    return Binder(key, topic, Sub, queue_size=queue_size)


def subBinderUDS(key, topic, fname, queue_size=5):
    return Binder(key, topic, Sub, fname=fname, queue_size=queue_size)
