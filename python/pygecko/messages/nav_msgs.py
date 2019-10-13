##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
from collections import namedtuple
from pygecko.messages.id import GeckoMsgFlags


class pose_t(namedtuple('pose_t', 'position orientation')):
    __slots__ = ()

    def __new__(cls, p, o):
        cls.id = GeckoMsgFlags.pose
        return cls.__bases__[0].__new__(cls, p, o)
