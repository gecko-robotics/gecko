#!/usr/bin/env python3

import ctypes
from ctypes import *
# from numpy.ctypeslib import ndpointer

charptr = POINTER(c_char)

gecko = ctypes.cdll.LoadLibrary('../../build/libgecko.so')
print(dir(gecko))
print(gecko._name)

gecko.zmqTCPc.argtypes = [charptr, int]
gecko.zmqTCPc.restype = charptr

print(gecko.zmqTCPc("localhost", 11311))
