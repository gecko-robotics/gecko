#!/usr/bin/env python
##############################################
# The MIT License (MIT)
# Copyright (c) 2018 Kevin Walchko
# see LICENSE for full details
##############################################
#
# https://pymotw.com/2/socket/multicast.html
#
import socket
import struct
# import threading
import time
import sys
# import ipaddress  # kjw
# from pygecko.pycore.ip import GetIP  # dup?
# from pygecko.pycore.transport import Ascii  # , Json, Pickle  # dup?
# import os
# import psutil
# import multiprocessing as mp


class BeaconBase(object):
    """
    https://www.tldp.org/HOWTO/Multicast-HOWTO-2.html
    TTL  Scope
    ----------------------------------------------------------------------
       0 Restricted to the same host. Won't be output by any interface.
       1 Restricted to the same subnet. Won't be forwarded by a router.
     <32 Restricted to the same site, organization or department.
     <64 Restricted to the same region.
    <128 Restricted to the same continent.
    <255 Unrestricted in scope. Global.
    """
    # mcast_addr = '224.3.29.110'
    mcast_addr = '224.0.0.1'
    mcast_port = 11311
    timeout = 2
    ttl = 1

    def __init__(self, ttl=2):
        self.group = (self.mcast_addr, self.mcast_port)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

        packed_ttl = struct.pack('b', ttl)
        self.sock.setsockopt(socket.SOL_IP, socket.IP_MULTICAST_TTL, packed_ttl)
        self.sock.setsockopt(socket.SOL_IP, socket.IP_MULTICAST_LOOP, 1)
        # self.key = key
        # self.host_ip = GetIP().get()
        # self.pid = mp.current_process().pid


class BeaconCoreServer(BeaconBase):
    def __init__(self, ttl=1, addr=None, print=True, timeout=None):
        BeaconBase.__init__(self, ttl=ttl)

        if addr:
            if len(addr) == 2:
                self.mcast_addr = addr[0]
                self.mcast_port = addr[1]

        # print performance to commandline
        # self.print = print

        # setup service socket
        # allow multiple connections
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        try:
            self.sock.bind(('', self.mcast_port))
            self.sock.settimeout(timeout)
        except OSError as e:
            print("*** {} ***".format(e))
            raise

        mreq = struct.pack("=4sl", socket.inet_aton(self.mcast_addr), socket.INADDR_ANY)
        self.sock.setsockopt(socket.SOL_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    def __del__(self):
        mreq = struct.pack("=4sl", socket.inet_aton(self.mcast_addr), socket.INADDR_ANY)
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)
        self.sock.close()

    def sendto(self, msg, address):
        self.sock.sendto(msg.encode("utf-8"), address)

    def cast(self, msg):
        self.sock.sendto(msg.encode("utf-8"), (self.mcast_addr, self.mcast_port))

    def recv(self):
        return self.sock.recvfrom(128)


msg = sys.argv[1]
mc = BeaconCoreServer(1)

try:
    while True:
        mc.cast(msg)
        print(">> beacon sent: {}".format(msg))
        data, address = mc.recv()
        data = data.decode("utf-8")
        if data != msg:
            print(">> {} from {}".format(data, address))
        else:
            print("** echo")
        time.sleep(1)

except KeyboardInterrupt:
    print("ctrl-z")
# except Exception as e:
#     print("***", e, "***")
