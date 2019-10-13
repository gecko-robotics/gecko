##############################################
# The MIT License (MIT)
# Copyright (c) 2019 Kevin Walchko
# see LICENSE for full details
##############################################
from pygecko.messages.std_msgs import Log
from colorama import Fore, Style
import time


class Logger:
    def __init__(self):
        pass

    def __format_print(self, topic, msg):
        # print(msg.level)
        # msg format: {proc_name, level, text}
        if msg.level == 'DEBUG': color = Fore.CYAN
        elif msg.level == 'WARN': color = Fore.YELLOW
        elif msg.level == 'ERROR': color = Fore.RED
        else: color = Fore.GREEN

        # shorten proc names??
        print(Style.BRIGHT + color + '>> {}:'.format(msg.name[:10]) + Style.RESET_ALL + msg.text)
        # print(">> {}: {}".format(topic, msg))

    def log(self, topic, msg):
        if self.logpub is None:
            self.__format_print(topic, msg)
        else:
            self.logpub.publish(msg)
