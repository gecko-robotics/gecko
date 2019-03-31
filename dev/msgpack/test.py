#!/usr/bin/env python3
# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2018 Kevin Walchko
# see LICENSE for full details
##############################################

from pygecko.multiprocessing import geckopy
from pygecko.transport import zmqUDS, Sub, Pub
from pygecko.messages import vec_t, imu_st, lidar_st
import time


# def chew_up_cpu(interval):
#     # chew up some cpu
#     start = time.time()
#     while (time.time() - start) < interval: 5*5


uds = zmqUDS("/tmp/0")

def publisher(**kwargs):
    geckopy.init_node(**kwargs)
    rate = geckopy.Rate(2)

    topic = kwargs.get('topic')
    p = Pub()
    p.bind(uds)

    while not geckopy.is_shutdown():
        a = vec_t(1,2,3)
        g = vec_t(4,5,6)
        m = vec_t(7,8,9)
        msg = imu_st(a,g,m)

        p.publish(msg)

        geckopy.logdebug('>> published msg')
        rate.sleep()
    print('pub bye ...')

def subscriber(**kwargs):
    geckopy.init_node(**kwargs)
    rate = geckopy.Rate(2)

    topic = kwargs.get('topic')
    # c = Callback(topic)
    s = Sub()
    s.topics = kwargs.get('topics')
    s.connect(uds)

    if s is None:
        raise Exception("subscriber is None")

    while not geckopy.is_shutdown():
        msg = s.recv_nb()
        if msg:
            print('='*50)
            geckopy.loginfo("{}: {}".format(topic,msg))
            print('-'*50)
            geckopy.loginfo("{}: {}".format(topic,msg.linear_accel))
            geckopy.loginfo("{}: {}".format(topic,msg.angular_vel))
        # chew_up_cpu(.1)
        # print(">> {}".format(msg))
        rate.sleep()

    print('sub bye ...')

def publisher_lidar(**kwargs):
    geckopy.init_node(**kwargs)
    rate = geckopy.Rate(2)

    topic = kwargs.get('topic')
    p = Pub()
    p.bind(uds)

    while not geckopy.is_shutdown():
        a = vec_t(1,2,3)
        g = vec_t(4,5,6)
        m = vec_t(7,8,9)
        msg = lidar_st(((1,2),(3.4,5.6)))

        p.publish(msg)

        geckopy.logdebug('>> published msg')
        rate.sleep()
    print('pub bye ...')


def subscriber_lidar(**kwargs):
    geckopy.init_node(**kwargs)
    rate = geckopy.Rate(2)

    topic = kwargs.get('topic')
    # c = Callback(topic)
    s = Sub()
    s.topics = kwargs.get('topics')
    s.connect(uds)

    if s is None:
        raise Exception("subscriber is None")

    while not geckopy.is_shutdown():
        msg = s.recv_nb()
        if msg:
            print('='*50)
            geckopy.loginfo("{}: {}".format(topic,msg))
            print('-'*50)
            # geckopy.loginfo("{}: {}".format(topic,msg.linear_accel))
            # geckopy.loginfo("{}: {}".format(topic,msg.angular_vel))
        # chew_up_cpu(.1)
        # print(">> {}".format(msg))
        rate.sleep()

    print('sub bye ...')


if __name__ == '__main__':
    # normally you wouldn't run this here, but is running else where
    # this is just for testing
    # from pygecko.transport import GeckoCore
    # core = GeckoCore()
    # core.start()

    # although I don't do anything with procs, because I reuse the variables
    # p and s below, they will kill the processes when new process are created
    # using those names. Appending them to procs allows me to keep them alive
    # until the program ends
    procs = []

    # info to pass to processes
    args = {
        "key": "dalek",
        'topic': "bob"
    }

    # subscriber(**args)
    # publisher(**args)
    # subscriber_lidar(**args)
    publisher_lidar(**args)

    #
    # p = GeckoSimpleProcess()
    # p.start(func=publisher, name='pub_{}'.format(topic), kwargs=args)
    # procs.append(p)
    #
    # s = GeckoSimpleProcess()
    # s.start(func=subscriber, name='sub_{}'.format(topic), kwargs=args)
    # procs.append(s)
    #
    # while True:
    #     try:
    #         time.sleep(1)
    #     except KeyboardInterrupt:
    #         print('main process got ctrl-c')
    #         break
