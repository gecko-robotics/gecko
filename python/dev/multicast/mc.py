#!/usr/bin/env python

import sys
import socket
import struct

DEFAULT_GROUP = '224.0.0.1'
DEFAULT_PORT = 49150


def send(data, *, group=DEFAULT_GROUP, port=DEFAULT_PORT, ttl=None):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    if ttl is not None:
        packed_ttl = struct.pack('b', ttl)
        s.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, packed_ttl)
    try:
        s.sendto(data, (group, port))
    finally:
        s.close()


def receive(*, group=DEFAULT_GROUP, port=DEFAULT_PORT, timeout=None):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    try:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        except AttributeError:
            # not available on Windows
            pass
        s.bind(('', port))

        s.settimeout(timeout)

        mreq = struct.pack('4sl', socket.inet_aton(group), socket.INADDR_ANY)
        s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
        try:
            data, sender_addr = s.recvfrom(4096)
        finally:
            s.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)
    finally:
        s.close()
    return data, sender_addr


if len(sys.argv) == 2:
    cmd = sys.argv[1]
    if cmd == "send":
        send(b"hello")
    elif cmd == "listen":
        print(receive())
    else:
        print("unknown command:", cmd)
