#!/usr/bin/env python3
# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################

import argparse
from argparse import RawTextHelpFormatter
import sys
import socket
from pprint import pprint
from pygecko.pycore.mbeacon import BeaconCoreServer
from pygecko.pycore.transport import Ascii

bag_help = """
    bag help
    something
    something else

    let's have some space   in the   help msg
"""

def handleArgs():
    parser = argparse.ArgumentParser(description='test program', formatter_class=RawTextHelpFormatter)
    # parser.add_argument('file', help='launch file')
    # parser.add_argument('-f', '--format', help='format: json or yaml, default is json', default='json')
    subparsers = parser.add_subparsers()

    # Bag ------------------------------------------------------------------
    b_p = subparsers.add_parser('bag', description=bag_help, formatter_class=RawTextHelpFormatter)
    b_p.set_defaults(which='bag')
    b_p.add_argument("mode", help="play,record")
    b_p.add_argument("filename", help="file name of bag file")
    b_p.add_argument("topic", help="topic name or 'all' for all topics")
    b_p.add_argument("-l", '--loop', help="loop during playback")
    b_p.add_argument("-t", "--type", help="msgpack or pickle for storage", default="pickle")

    # Topic -----------------------------------------------------------------
    t_p = subparsers.add_parser('topic', description='topic help')
    t_p.set_defaults(which='topic')
    t_p.add_argument("mode", help="echo, bw, pub, list")
    t_p.add_argument("topic", help="topic name")
    # t_p.add_argument("-l", '--loop', help="loop")

    # Multicast --------------------------------------------------------------
    m_p = subparsers.add_parser('multicast', description='multicast help')
    m_p.set_defaults(which='multicast')
    m_p.add_argument("mode", help="send or receive")
    # m_p.add_argument("topic")

    # Core ------------------------------------------------------------------
    c_p = subparsers.add_parser('core', description='core help')
    c_p.set_defaults(which='core')
    c_p.add_argument("mode", help="run, alive, performance")

    # Launch ---------------------------------------------------------------
    l_p = subparsers.add_parser('launch', description='launch help')
    l_p.add_argument('file', help="file name of launch file")
    l_p.set_defaults(which='launch')
    l_p.add_argument('-f', '--format', help='format: json or yaml, default is json', default='json')

    # Service --------------------------------------------------------------
    s_p = subparsers.add_parser('service', description='service help')
    s_p.set_defaults(which='service')

    # Gecko ----------------------------------------------------------------
    parser.add_argument('-k', '--key', help="key for the geckocore, default is hostname", default=None)
    args = vars(parser.parse_args())
    return args

def run_core(key):
    if key is None:
        key = socket.gethostname().lower().split('.')[0]

    bs = BeaconCoreServer(key=key, handler=Ascii)
    bs.start()
    bs.run()

if __name__ == "__main__":
    args = handleArgs()
    pprint(args)
    # print(sys.argv[1])
    # exit(0)

    mode = args["which"]
    if mode == 'bag':
        print("bag {} {}".format(args['mode'], args['filename']))
    elif mode == 'topic':
        print("topic")
    elif mode == 'multicast':
        print("multicast")
    elif mode == 'core':
        if args["mode"] == "run": run_core(args['key'])
        # print("core")
    elif mode == "launch":
        print("launch")
    elif mode == "service":
        print("service")
