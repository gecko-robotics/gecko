##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
from collections import namedtuple
from pygecko.messages.id import GeckoMsgFlags
import time


class imu_st(namedtuple('imu_st', 'linear_accel angular_vel magnetic_field timestamp')):
    """
    Inertial measurement unit
    """
    __slots__ = ()

    def __new__(cls, a, g, m, ts=None):
        cls.id = GeckoMsgFlags.imu
        if ts:
            return cls.__bases__[0].__new__(cls, a, g, m, ts)
        else:
            return cls.__bases__[0].__new__(cls, a, g, m, time.time())


class joystick_st(namedtuple('joystick_st', 'axes buttons type timestamp')):
    """
    Joystick: axes buttons type timestamp
    """
    __slots__ = ()

    def __new__(cls, a, b, t, ts=None):
        cls.id = GeckoMsgFlags.joystick
        if ts:
            return cls.__bases__[0].__new__(cls, a, b, t, ts)
        else:
            return cls.__bases__[0].__new__(cls, a, b, t, time.time())


class lidar_st(namedtuple('lidar_st', 'data timestamp')):
    """
    Laser range finder
    """
    __slots__ = ()

    def __new__(cls, s, ts=None):
        cls.id = GeckoMsgFlags.lidar
        if ts:
            return cls.__bases__[0].__new__(cls, s, ts)
        else:
            return cls.__bases__[0].__new__(cls, s, time.time())


try:
    import cv2
    import numpy as np

    class image_st(namedtuple('image_st', 'shape bytes compressed timestamp')):
        """
        This is the message passed from process to process.
        shape: (rows, cols, planes)
        bytes: array of bytes that are or aren't compressed
        compressed: are the bytes compressed? True/False
        timestamp: unix timestamp
        """
        __slots__ = ()

        def __new__(cls, s, b, c, ts=None):
            cls.id = GeckoMsgFlags.image

            if ts:
                return cls.__bases__[0].__new__(cls, s, b, c, ts)
            else:
                return cls.__bases__[0].__new__(cls, s, b, c, time.time())

        def uncompress(self):
            img = np.frombuffer(self.bytes, dtype=np.uint8)

            if self.compressed:
                if len(self.shape) == 3:
                    img = cv2.imdecode(img, cv2.IMREAD_COLOR)
                else:
                    img = cv2.imdecode(img, cv2.IMREAD_GRAYSCALE)

            img = img.reshape(self.shape)
            return img

except ImportError:

    class image_st(namedtuple('image_st', 'shape bytes compressed timestamp')):
        __slots__ = ()

        def __new__(cls, s, b, c, ts=None):
            cls.id = GeckoMsgFlags.image
            if ts:
                return cls.__bases__[0].__new__(cls, s, b, c, ts)
            else:
                return cls.__bases__[0].__new__(cls, s, b, c, time.time())

        def uncompress(self):
            raise Exception("image_st::uncompress: cv2 not installed")


# Services -----------------------------------------------------------

class camera_info_t(namedtuple('camera_info_t', 'size distortion D K R P')):
    """
    Camera information
    """
    __slots__ = ()

    def __new__(cls, s, d, D, K, R, P):
        cls.id = GeckoMsgFlags.camera_info
        return cls.__bases__[0].__new__(cls, s, d, D, K, R, P)
