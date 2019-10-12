#!/usr/bin/env python3
# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
# import time
# from pygecko.transport import GeckoCore
#
#
# if __name__ == "__main__":
#     core = GeckoCore()
#     core.start()
#
#     try:
#         while True:
#             time.sleep(1)
#     except KeyboardInterrupt:
#         pass
# import threading
# import time
import argparse
import sys
import socket
from pygecko.pycore.mbeacon import BeaconCoreServer
from pygecko.pycore.transport import Ascii


def handleArgs():
    parser = argparse.ArgumentParser(description='test program')
    # parser.add_argument('file', help='launch file')
    # parser.add_argument('-f', '--format', help='format: json or yaml, default is json', default='json')
    tool = None
    subparsers = parser.add_subparsers()

    # Bag ------------------------------------------------------------------
    b_p = subparsers.add_parser('bag', description='bag help')
    b_p.add_argument("mode", help="play,record")
    b_p.add_argument("filename")
    b_p.add_argument("topic")
    b_p.add_argument("-l", '--loop', help="loop")

    # Topic -----------------------------------------------------------------
    t_p = subparsers.add_parser('topic', description='topic help')
    t_p.add_argument("mode", help="echo, bw, pub")
    t_p.add_argument("topic")
    # t_p.add_argument("-l", '--loop', help="loop")

    # Multicast --------------------------------------------------------------
    m_p = subparsers.add_parser('multicast', description='multicast help')
    m_p.add_argument("mode", help="send or receive")
    # m_p.add_argument("topic")

    # Core ------------------------------------------------------------------
    m_p = subparsers.add_parser('core', description='core help')
    tool = "core"
    m_p.add_argument("mode", help="run or alive")

    # Gecko ----------------------------------------------------------------
    parser.add_argument('-k', '--key', help="key for the geckocore, default is hostname", default=None)
    args = vars(parser.parse_args())
    args["tool"] = tool
    return args

def run_core(key):
    if key is None:
        key = socket.gethostname().lower().split('.')[0]

    bs = BeaconCoreServer(key=key, handler=Ascii)
    bs.start()
    bs.run()

if __name__ == "__main__":
    args = handleArgs()
    print(args)
    print(sys.argv[1])

    mode = args["tool"]
    if mode == 'bag':
        print("bag {} {}".format(args['mode'], args['filename']))
    elif mode == 'topic':
        print("topic")
    elif mode == 'multicast':
        print("multicast")
    elif mode == 'core':
        run_core(args['key'])
