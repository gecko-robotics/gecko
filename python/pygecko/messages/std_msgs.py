##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
from collections import namedtuple
from pygecko.messages.id import GeckoMsgFlags

Log = namedtuple('Log', 'level name text')

class vec_t(namedtuple('vec_t', 'x y z')):
    __slots__ = ()

    def __new__(cls, x, y, z):
        cls.id = GeckoMsgFlags.vector
        return cls.__bases__[0].__new__(cls, x, y, z)


class quaternion_t(namedtuple('quaternion_t', 'w x y z')):
    __slots__ = ()

    def __new__(cls, w, x, y, z):
        cls.id = GeckoMsgFlags.quaternion
        return cls.__bases__[0].__new__(cls, w, x, y, z)


class wrench_t(namedtuple('wrench_t', 'force torque')):
    __slots__ = ()

    def __new__(cls, f, t):
        cls.id = GeckoMsgFlags.wrench
        return cls.__bases__[0].__new__(cls, f, t)


class twist_t(namedtuple('twist_t', 'linear angular')):
    __slots__ = ()

    def __new__(cls, l, a):
        cls.id = GeckoMsgFlags.twist
        return cls.__bases__[0].__new__(cls, l, a)
