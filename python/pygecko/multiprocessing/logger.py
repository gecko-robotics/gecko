# -*- coding: utf-8 -*-
##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
from colorama import Fore, Back, Style
import logging
import logging.handlers


class CustomFormatter(logging.Formatter):
    """Logging Formatter to add colors and count warning / errors"""

    reset = Style.RESET_ALL

    FORMATS = {
        logging.DEBUG:    Back.GREEN + Fore.WHITE + "[%(asctime)s DBUG>" + reset + " %(message)s",
        logging.INFO:     Back.BLUE + Fore.WHITE + "[%(asctime)s INFO>" + reset + " %(message)s",
        logging.WARNING:  Back.YELLOW + Fore.WHITE + "[%(asctime)s WARN>" + reset + " %(message)s",
        logging.ERROR:    Back.RED + Fore.WHITE + "[%(asctime)s ERR>" + reset + " %(message)s",
        logging.CRITICAL: Back.MAGENTA + Fore.WHITE + "[%(asctime)s CRIT>" + reset + " %(message)s"
    }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt, datefmt="%H:%M:%S")
        return formatter.format(record)


def getLogger(name, filename=None, level=logging.DEBUG):
    """
    name: logger name to get
    filename: name of log file to append to. If None (default), no file.
    level: minimum level to log to (debug is the lowest)
    """
    logger = logging.getLogger(name)
    logger.setLevel(level)

    # stdout -----------------------------------
    ch = logging.StreamHandler()
    ch.setFormatter(CustomFormatter())
    logger.addHandler(ch)

    # file -------------------------------------
    if filename:
        fh = logging.FileHandler(filename)
        formatter = logging.Formatter('%(asctime)s %(levelname)s: %(message)s', datefmt="%Y-%m-%d %H:%M:%S")
        fh.setFormatter(formatter)
        logger.addHandler(fh)

    return logger
