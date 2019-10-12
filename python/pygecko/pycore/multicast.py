import sys
import socket
import struct

DEFAULT_GROUP = '224.0.0.1'
DEFAULT_PORT = 49150


class MCast:
    def __init__(self, group=DEFAULT_GROUP, port=DEFAULT_PORT, timeout=None, ttl=None):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.group = group
        self.port = port
        self.timeout = timeout
        if not ttl:
            ttl = 2
        self.ttl = ttl

    def __del__(self):
        self.close()

    def close(self):
        self.sock.close()

    def send(self, data, *):
        # s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        if ttl is not None:
            packed_ttl = struct.pack('b', ttl)
            self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, packed_ttl)
        try:
            self.sock.sendto(data, (group, port))
        # finally:
        #     self.sock.close()

    def receive(self):
        # s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        try:
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            try:
                self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
            except AttributeError:
                # not available on Windows
                pass
            self.sock.bind(('', port))

            self.sock.settimeout(timeout)

            mreq = struct.pack('4sl', socket.inet_aton(group), socket.INADDR_ANY)
            self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
            try:
                data, sender_addr = self.sock.recvfrom(4096)
            finally:
                self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)
        # finally:
        #     self.sock.close()
        return data, sender_addr


class SimpleSend(MCast):
    def __init__(self):
        self.__init__()

    def send(self, data, *):
        if ttl is not None:
            packed_ttl = struct.pack('b', ttl)
            self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, packed_ttl)
        try:
            self.sock.sendto(data, (group, port))
        # finally:
        #     self.sock.close()
