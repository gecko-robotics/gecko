# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
import multiprocessing as mp
from pygecko.multiprocessing.sig import SignalCatch
from pygecko.transport.zmq_base import get_ip


class GeckoPySingleton(SignalCatch):
    """
    This class setups a function in a new process.
    """
    def __init__(self, **kwargs):
        """
        kwargs: can have a lot of things in it. Some common keys are:
            core_outaddr: tcp or uds address of geckocore outputs
            core_inaddr: tcp or uds address of geckocore inputs
            queue: multiprocessing.Queue for log messages
        """
        # geckopy info
        self.kill_signals()  # have to setup signals in new process
        # self.subs = []   # subscriber nodes
        # self.srvs = []   # services
        # self.hooks = []  # functions to call on shutdown
        self.name = mp.current_process().name
        # self.pid = mp.current_process().pid
        self.logpub = None

        self.binders = {}  # topic/endpt

        self.proc_ip = get_ip()  # this ip address

    def print_info(self):
        print("----------------------------------")
        print("GeckoPy [{}]".format(self))
        print("-----------")
        print("  Process:", self.name)
        print("  PID:", mp.current_process().pid)
        print("  Host: {}".format(self.proc_ip))
        print("----------------------------------")
