# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################

import yaml
import os
try:
    import simplejson as json
except ImportError:
    import json


class FileStorageError(Exception):
    pass


class BaseFile(object):
    def read(self, fname, func):
        try:
            with open(fname, 'r') as fd:
                data = func(fd)

        except IOError:
            raise FileStorageError('Could not open {0!s} for reading'.format((fname)))

        return data

    def getFileSize(self, fname):
        return os.path.getsize(fname)

    def write(self, fname, func, data):
        try:
            with open(fname, 'w') as fd:
                func(data, fd)

        except IOError:
            raise FileStorageError('Could not open {0!s} for writing'.format((fname)))


class FileJson(BaseFile):
    def read(self, fname):
        """
        Reads a Json file
        in: file name
        out: length of file, dictionary
        """
        return super().read(fname, json.load)

    def write(self, fname, data):
        """
        Writes a Json file
        """
        super().write(fname, json.dump, data)


class FileYaml(BaseFile):
    def read(self, fname):
        """
        Reads a Yaml file
        in: file name
        out: length of file, dictionary
        """
        return super().read(fname, yaml.safe_load)

    def write(self, fname, data):
        """
        Writes a Yaml file
        in:
            filename - file name
            data - [optional] data to be written, otherwise, it uses data in self.db
        """
        super().write(fname, yaml.safe_dump, data)
