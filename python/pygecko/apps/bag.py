# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
from the_collector import BagIt
from the_collector import Pickle


class BagRecord:
    def __init__(self):
        self.bag = BagIt(Pickle)
        self.data = None

    def read(self, fname):
        self.data = self.bag.read(fname)

    def save(self):
        pass

    def play(self, loop=False):
        if loop:
            while True:
                self.play_once()
        else:
            self.play_once()

    def play_once(self):
        if self.data is None:
            raise Exception("Bag::play: you must open ")
