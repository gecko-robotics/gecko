#!/usr/bin/env python3

from colorama import Fore, Back, Style
import logging
import logging.handlers


class CustomFormatter(logging.Formatter):
    """Logging Formatter to add colors and count warning / errors"""

    # grey = Fore.CYAN
    # yellow = Fore.YELLOW
    # red = Fore.RED
    # bold_red = Fore.MAGENTA + Style.BRIGHT
    reset = Style.RESET_ALL
    # format = "%(asctime)s [%(levelname)s]:  %(message)s "

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


def getLogger(name, filename=None):
    logger = logging.getLogger(name)
    logger.setLevel(logging.DEBUG)

    ch = logging.StreamHandler()
    ch.setFormatter(CustomFormatter())
    logger.addHandler(ch)

    if filename:
        fh = logging.FileHandler(filename)
        formatter = logging.Formatter('%(asctime)s %(levelname)s: %(message)s', datefmt="%Y-%m-%d %H:%M:%S")
        fh.setFormatter(formatter)
        logger.addHandler(fh)

    return logger


logger = getLogger(__name__)

logger.debug('debug message 2')
logger.info('info message')
logger.warning('warning message')
logger.error('error message')
logger.critical('critical message')
