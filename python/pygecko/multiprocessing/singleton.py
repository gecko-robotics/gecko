
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

        # hard code for now
        # if 'host' in kwargs.keys():
        #     host = kwargs.pop('host')
        #     if host == 'localhost':
        #         host = GetIP().get()
        # else:
        # host = GetIP().get()  # FIXME: kwargs should provide this
        # self.req_addr = zmqTCP(host, 11311)  # set/get topic addrs
        self.proc_ip = get_ip()  # this ip address

        print("----------------------------------")
        print("GeckoPy [{}]".format(self))
        print("-----------")
        print("  Process:", self.name)
        print("  PID:", mp.current_process().pid)
        print("  Host: {}".format(self.proc_ip))
        print("----------------------------------")

    # def __del__(self):
    #     if len(self.hooks) > 0:
    #         for h in self.hooks:
    #             h()

    # def __format_print(self, topic, msg):
    #     # print(msg.level)
    #     # msg format: {proc_name, level, text}
    #     if msg.level == 'DEBUG': color = Fore.CYAN
    #     elif msg.level == 'WARN': color = Fore.YELLOW
    #     elif msg.level == 'ERROR': color = Fore.RED
    #     else: color = Fore.GREEN
    #
    #     # shorten proc names??
    #     print(Style.BRIGHT + color + '>> {}:'.format(msg.name[:10]) + Style.RESET_ALL + msg.text)
    #     # print(">> {}: {}".format(topic, msg))
    #
    # def log(self, topic, msg):
    #     if self.logpub is None:
    #         self.__format_print(topic, msg)
    #     else:
    #         self.logpub.publish(msg)
