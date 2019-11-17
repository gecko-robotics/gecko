##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
#
# Kevin J. Walchko 13 Oct 2014
#
# see http://zeromq.org for more info
# http://zguide.zeromq.org/py:all\
import zmq
from pygecko.network.ip import get_ip


class ZMQError(Exception):
    pass


def zmq_version():
    """
    What version of the zmq (C++) library is python tied to?
    """
    return 'Using ZeroMQ version: {0!s}'.format((zmq.zmq_version()))


def zmqTCP(host, port=None):
    """
    Set the zmq address as TCP: tcp://host:port
    """
    if host == 'localhost':
        # host = GetIP().get()
        host = get_ip()
    if port:
        ret = 'tcp://{}:{}'.format(host, port)
    else:
        ret = 'tcp://{}'.format(host)
    return ret


def zmqUDS(mnt_pt):
    """
    Set the zmq address as a Unix Domain Socket: ipc://file
    """
    return 'ipc://{}'.format(mnt_pt)


class Base(object):
    """
    Base class for other derived pub/sub/service classes
    """
    # ctx = zmq.Context()
    # socket = None
    # pack = None
    # topics = None

    # def __init__(self, kind, serialize=MsgPack):
    def __init__(self, kind):
        self.topics = None
        self.pack = None  # ???
        self.ctx = zmq.Context(1)

        self.timeout = 1000

        self.socket = self.ctx.socket(kind)

        # subscribers don't seem to work for some reason
        # if kind in [zmq.SUB, zmq.REQ, zmq.REP]:
        if kind in [zmq.REQ, zmq.REP]:
            # print(">> Created poller for:", kind)
            self.poller = zmq.Poller()
            self.poller.register(self.socket, zmq.POLLIN)
        else:
            self.poller = None

    def __del__(self):
        """Calls close()"""
        self.close()
        # self.ctx.term()
        # self.socket.close()
        # print('[<] shutting down {}'.format(type(self).__name__))

    def close(self):
        """Closes socket and terminates context"""
        if self.socket:
            # see if we created a poller
            if self.poller:
                self.poller.unregister(self.socket)

            self.socket.setsockopt(zmq.LINGER, 0)
            self.socket.close()
        if self.ctx:
            self.ctx.term()
        # print('[<] shutting down {}'.format(type(self).__name__))

    def bind(self, addr, hwm=None, queue_size=10, random=False):
        """
        Binds a socket to an addr. Only one socket can bind.
        Usually pub binds and sub connects, but not always!

        args:
          addr as tcp or uds
            uds: ipc://<file_path>
            tcp:
                known: tcp://x.x.x.x:port
                random: tcp://x.x.x.x:*
          hwm (high water mark) a int that limits buffer length
          queue_size is the same as hwm
          random: select a random port to bind to
        return: port number
        """
        # print(type(self).__name__, 'bind to {}'.format(addr))
        if random:
            # https://pyzmq.readthedocs.io/en/latest/api/zmq.html#zmq.Socket.bind_to_random_port
            port = self.socket.bind_to_random_port(addr)  # tcp://* ???
        else:
            # uds = False
            # print(">>", addr)
            if addr.find("ipc") >= 0:
                # uds = True
                # if uds:
                self.socket.bind(addr)
                port = None
            else:
                port = int(addr.split(':')[2])  # tcp://ip:port
                self.socket.bind(addr)

        if hwm:
            self.socket.set_hwm(hwm)
        elif queue_size:
            self.socket.set_hwm(queue_size)

        return port

    def connect(self, addr, hwm=None, queue_size=10):
        """
        Connects a socket to an addr. Many different sockets can connect.
        Usually pub binds and sub connects, but not always!

        args:
            addr as tcp or uds
            hwm (high water mark) a int that limits buffer length
            queue_size is the same as hwm
        return: none
        """
        # print(type(self).__name__, 'connect to {}'.format(addr))
        self.socket.connect(addr)
        if hwm:
            self.socket.set_hwm(hwm)
        elif queue_size:
            self.socket.set_hwm(queue_size)

    def recv_poll(self):
        """
        Performs a polling receive on the socket.
        """
        try:
            # Determine if there is something to read (zmq.POLLIN):
            # socks = {<zmq.sugar.socket.Socket object at 0x7f973d36d660>: 1}
            socks = dict(self.poller.poll(timeout=self.timeout))
            # print(socks)
            if socks.get(self.socket) == zmq.POLLIN:
                return self.socket.recv(flags=0)
            else:
                # print("*** recv_poll got none ***")
                return None

        except zmq.Again as e:
            # no response yet or server not up and running yet
            time.sleep(0.001)
        except Exception as e:
            # something else is wrong
            print("*** Oops: {} ***".format(e))
            raise
        return None
