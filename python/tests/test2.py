import time
import os
from collections import namedtuple
from multiprocessing import Event
from math import pi

from pygecko.apps.core import CoreServer
from pygecko.network.transport import Ascii

from pygecko.multiprocessing import geckopy
from pygecko.transport.zmq_sub_pub import Pub, Sub
from pygecko.multiprocessing.process import GeckoSimpleProcess

import numpy as np
# from pygecko.proto.standard_msgs_pb2 import Vector
# from pygecko.proto.sensor_msgs_pb2 import Imu, Image
from pygecko.single.msgs_pb2 import Imu, Image
import time


def test_msg_zmq():
    imu = Imu()
    imu.linear_acceleration.x = 1
    imu.linear_acceleration.y = 11
    imu.linear_acceleration.z = 111
    imu.angular_velocity.x = 2
    imu.angular_velocity.y = 22.22
    imu.angular_velocity.z = 222.22
    imu.magnetic_field.x = 3
    imu.magnetic_field.y = 33
    imu.magnetic_field.z = 333
    imu.orientation.w = 1
    imu.orientation.x = 0
    imu.orientation.y = 0
    imu.orientation.z = 0
    imu.timestamp = time.time()

    s = imu.SerializeToString()

    imuu = Imu()
    imuu.ParseFromString(s)
    assert imu == imuu, "Imu is different"



def zmq_pub_sub(args):
    geckopy.init_node()
    # stop pub
    exit = Event()
    exit.clear()

    msg = Imu()
    msg.linear_acceleration.x = 1
    msg.linear_acceleration.y = 11
    msg.linear_acceleration.z = 111
    msg.angular_velocity.x = 2
    msg.angular_velocity.y = 22.22
    msg.angular_velocity.z = 222.22
    msg.magnetic_field.x = 3
    msg.magnetic_field.y = 33
    msg.magnetic_field.z = 333
    msg.orientation.w = 1
    msg.orientation.x = 0
    msg.orientation.y = 0
    msg.orientation.z = 0
    msg.timestamp = time.time()

    def publisher(**kwargs):
        geckopy.init_node()
        exit = kwargs['exit']

        pt = kwargs["pub"]
        key = kwargs["key"]
        topic = kwargs["topic"]
        if pt == "bindtcp":
            p = geckopy.pubBinderTCP(key, topic)
        elif pt == "connecttcp":
            p = geckopy.pubConnectTCP(key, topic)
        elif pt == "binduds":
            p = geckopy.pubBinderUDS(key, topic, "/tmp/pygecko_test")
        elif pt == "connectuds":
            p = geckopy.pubConnectUDS(key, topic)

        if p is None:
            assert False, "<<< Couldn't get Pub from geckocore >>>"

        for _ in range(100):
            if exit.is_set():
                # print("exit")
                break
            p.publish(msg.SerializeToString())
            time.sleep(0.1)

    p = GeckoSimpleProcess()
    args['exit'] = exit
    p.start(func=publisher, name='publisher', kwargs=args)

    st = args["sub"]
    key = args["key"]
    topic = args["topic"]

    if st == "connecttcp":
        s = geckopy.subConnectTCP(key, topic)
    elif st == "bindtcp":
        s = geckopy.subBinderTCP(key, topic)
    elif st == "connectuds":
        s = geckopy.subConnectUDS(key, topic)
    elif st == "binduds":
        s = geckopy.subBinderUDS(key, topic, "/tmp/pygecko_test_2")

    for _ in range(5):
        mm = s.recv()

        if mm:
            exit.set()
            break
        else:
            print(".", end=" ", flush=True)
            time.sleep(0.1)
    m = Imu()
    m.ParseFromString(mm)
    assert msg == m, "{} => {}".format(msg, m)


def test_pub_sub():

    bs = CoreServer(key='test', handler=Ascii)
    core = GeckoSimpleProcess()
    core.start(func=bs.run, name='geckocore')

    args = {
        'key': 'test',
        'topic': "test-tcp",
        'pub': 'bindtcp',
        'sub': 'connecttcp'
    }
    zmq_pub_sub(args)

    args = {
        'key': 'test',
        'topic': "test-tcp-2",
        'pub': 'connecttcp',
        'sub': 'bindtcp'
    }
    zmq_pub_sub(args)

    args = {
        'key': 'test',
        'topic': "test-uds",
        'pub': 'binduds',
        'sub': 'connectuds'
    }
    zmq_pub_sub(args)

    args = {
        'key': 'test',
        'topic': "test-uds-2",
        'pub': 'connectuds',
        'sub': 'binduds'
    }
    zmq_pub_sub(args)

    bs.stop()
    core.join(0.1)
