#!/usr/bin/env python3
# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################

import argparse
from argparse import RawTextHelpFormatter
import time
import socket
from pprint import pprint
from pygecko.apps.core import CoreServer
# from pygecko.apps.core import Ascii
from pygecko.network.transport import Ascii
from pygecko.network.mcsocket import MultiCastSocket
from colorama import Fore, Style

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
    m_p.add_argument("mode", help="a, b or c")
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

    bs = CoreServer(key=key, handler=Ascii)
    bs.start()
    bs.run()


def run_multicast(address, port, msg):
    mc = MultiCastSocket(group=(address, port), ttl=2, timeout=1)
    # print(">>", mc.info())

    while True:
        try:
            # if msg != "receive": mc.cast(msg.encode("utf-8"))
            mc.cast(msg.encode("utf-8"))
            time.sleep(1)
            # print(">> beacon sent: {}".format(msg))
            data = msg
            while data == msg:
                data, address = mc.recv()
                if data:
                    data = data.decode("utf-8")
                time.sleep(0.005)
                # print("==>", data, data == msg)
            # data, address = mc.recv_nb()
            if data is None:
                # print("timeout")
                # time.sleep(1)
                continue

            # data = data.decode("utf-8")
            # if data != msg:
            print(">> " +Fore.CYAN + msg + Style.RESET_ALL + " -> " + Fore.GREEN + data + Style.RESET_ALL + " from:", address)
            # else:
            #     print(Fore.YELLOW + "*** see echo ***" + Style.RESET_ALL)
            # if msg != "receive":
            # time.sleep(1)

        except KeyboardInterrupt:
            print("ctrl-z")
            break
        # except Exception as e:
        #     print("***", e, "***")

def run_bag():
    pass


if __name__ == "__main__":
    args = handleArgs()
    # pprint(args)
    # print(sys.argv[1])
    # exit(0)

    mode = args["which"]
    if mode == 'bag':
        print("bag {} {}".format(args['mode'], args['filename']))
    elif mode == 'topic':
        print("topic")
    elif mode == 'multicast':
        print("multicast")
        msg = args['mode']
        if msg not in ["a", "b", "c"]:
            print("multicast: invalid mode:", msg)
            print("multicast mode must be either 'a' or 'b'")
            exit(1)
        run_multicast("224.0.0.1", 11311, msg)
    elif mode == 'core':
        if args["mode"] == "run": run_core(args['key'])
        # print("core")
    elif mode == "launch":
        print("launch")
    elif mode == "service":
        print("service")
