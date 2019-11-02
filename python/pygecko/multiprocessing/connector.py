# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
from pygecko.transport.zmq_sub_pub import Pub, Sub
import time
import multiprocessing as mp
from .mc_core_socket import BeaconFinder


def Connector(key, topic, Proto, queue_size=5):
    """
    Creates a publisher that can either connect or bind to an address.

    conn -> (key, topic, pid)
    conn <- (key, topic, endpt, ok) py
    conn <- (key, topic, endpt, ok) cpp

    key: geckocore key
    topic: pub/sub topic name
    Proto: either Pub or Sub
    queue_size: how many messages to queue up, default is 5
    """
    global g_geckopy

    bf = BeaconFinder(key)
    pid = mp.current_process().pid
    msg = (key, topic, str(pid))
    retry = 5
    data = None

    if topic in g_geckopy.binders:
        endpt = g_geckopy.binders[topic]
        p = Proto()
        p.connect(endpt)
        # print("geckopy.Connector SUCCESS")
        return p

    for i in range(retry):
        data = bf.send(msg)
        # print(">> conn raw: ", data)

        if data is None:
            time.sleep(0.5)
            continue

        if (len(data) == 4) and (data[0] == key) and (data[1] == topic) and data[3] == "ok":
            p = Proto()
            p.connect(data[2])
            # print("geckopy.Connector SUCCESS", i)
            return p

    return None


def pubConnectTCP(key, topic, queue_size=5):
    return Connector(key, topic, Pub, queue_size)


def pubConnectUDS(key, topic, queue_size=5):
    return Connector(key, topic, Pub, queue_size)


def subConnectTCP(key, topic, queue_size=5):
    return Connector(key, topic, Sub, queue_size)


def subConnectUDS(key, topic, queue_size=5):
    return Connector(key, topic, Sub, queue_size)
