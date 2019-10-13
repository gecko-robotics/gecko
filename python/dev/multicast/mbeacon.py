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
import select
import struct
import time
import sys


class MultiCastError(Exception):
    pass


class MultiCastSocket:
    """
    Multicast: 224.0.0.0 - 239.255.255.255
    
    224.0.0.1 is the all-hosts group. If you ping that group, all multicast
    capable hosts on the network should answer, as every multicast capable host
    must join that group at start-up on all it's multicast capable interfaces.

    group: (address, port)
    timeout: seconds to wait, 0 is blocking
    echo: 0 disable loopback, 1 enable loopback

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
    sock = None

    def __init__(self, ttl=1, group=None, timeout=None, echo=1):
        if group and len(group) == 2:
            self.mcast_addr = group[0]
            self.mcast_port = group[1]
            self.group = group
        else:
            # self.mcast_addr = '224.0.0.1'
            # self.mcast_port = 11311
            # self.group = ('224.0.0.1', 11311)
            raise MultiCastError("invalid group address")

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

        packed_ttl = struct.pack('b', ttl)
        self.sock.setsockopt(socket.SOL_IP, socket.IP_MULTICAST_TTL, packed_ttl)
        self.sock.setsockopt(socket.SOL_IP, socket.IP_MULTICAST_LOOP, echo)

        # setup service socket
        # allow multiple connections
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        try:
            self.sock.bind(('', self.mcast_port))
            self.sock.settimeout(timeout)
            # self.sock.setblocking(0)
        except OSError as e:
            print("*** {} ***".format(e))
            raise

        mreq = struct.pack("=4sl", socket.inet_aton(self.mcast_addr), socket.INADDR_ANY)
        self.sock.setsockopt(socket.SOL_IP, socket.IP_ADD_MEMBERSHIP, mreq)

    def __del__(self):
        if self.sock is None: return
        mreq = struct.pack("=4sl", socket.inet_aton(self.mcast_addr), socket.INADDR_ANY)
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)
        self.sock.close()

    def info(self):
        print(">> Multicast: {}:{}".format(*self.group))

    def sendto(self, msg, address):
        self.sock.sendto(msg.encode("utf-8"), address)

    def cast(self, msg):
        self.sock.sendto(msg.encode("utf-8"), self.group)

    def recv(self):
        try:
            ret = self.sock.recvfrom(128)
        except socket.timeout:
            ret = (None, None)
        return ret

    def recv_nb(self, tout=1):
        inputs = [self.sock]
        readable, _, _ = select.select(inputs, [], [], tout)
        # print("how many:", len(readable))
        ret = (None, None)
        for s in readable:
            if s == self.sock:
                ret = self.sock.recvfrom(128)
            else:
                print("not my sock")

        return ret


if len(sys.argv) != 2:
    print("Usage: ./mc.py message")

msg = sys.argv[1]
mc = MultiCastSocket(group=('224.0.0.1', 11311), ttl=2, timeout=0)
mc.info()

while True:
    try:
        if msg != "no": mc.cast(msg)
        # time.sleep(3)
        # print(">> beacon sent: {}".format(msg))
        # data, address = mc.recv()
        data, address = mc.recv_nb()
        if data is None:
            print("timeout")
            continue

        data = data.decode("utf-8")
        if data != msg:
            print(">> {} from {}".format(data, address))
        else:
            print("** echo")
        if msg != "no": time.sleep(3)
    except KeyboardInterrupt:
        print("ctrl-z")
        break
    # except Exception as e:
    #     print("***", e, "***")
