import time
from pygecko.network.transport import Ascii
from pygecko.network.mcsocket import MultiCastSocket

# class CoreTalker:
class BeaconFinder:
    """
    Find Services using the magic of multicast

    pid = 123456
    proc_name = "my-cool-process"

    key = hostname
    finder = BeaconFinder(key)
    msg = finder.search(msg)
    """
    def __init__(self, key, ttl=2, handler=Ascii):
        # BeaconBase.__init__(self, key=key, ttl=ttl)
        self.key = key
        self.group = ("224.0.0.1", 11311)
        self.sock = MultiCastSocket(group=self.group, ttl=ttl, timeout=2)
        self.handler = handler()

    def send(self, msg):
        """
        Search for services using multicast sends out a request for services
        of the specified name and then waits and gathers responses. This sends
        one mdns ping. As soon as a responce is received, the function returns.
        """
        # serviceName = 'GeckoCore'
        # self.sock.settimeout(self.timeout)
        # msg = self.handler.dumps((self.key, serviceName, str(pid), processname,))
        # msg['key'] = self.key

        key = msg[0]
        if key != self.key:
            raise Exception("invalid key:", key)

        topic = msg[1]

        msg = self.handler.dumps(msg)
        # print("msg:", msg)
        # self.sock.sendto(msg, self.group)
        self.sock.cast(msg)
        servicesFound = None
        time.sleep(0.01)
        # while True:
        # try:

        # data = returned message info
        # server = ip:port, which is x.x.x.x:9990
        data, server = self.sock.recv()

        if data is None:
            return None

        if msg == data:
            # print("send echo:", data, server)
            return None

        # print("beconfinder recv data:", data, server)

        if data:
            data = self.handler.loads(data)

            if len(data) == 4:
                # data = self.handler.loads(data.decode("utf-8"))
                # print("wtf:", data)
                # print('>> Search:', data, server)
                if data[3] == "ok" and data[0] == self.key and data[1] == topic:
                    servicesFound = data
                # else:
                #     print("FAIL:", topic, data)
                # break
        return servicesFound
