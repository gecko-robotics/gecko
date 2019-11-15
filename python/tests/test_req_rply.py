from multiprocessing import Event
import json

from pygecko.multiprocessing import geckopy
from pygecko.transport.zmq_sub_pub import Pub, Sub
from pygecko.multiprocessing.process import GeckoSimpleProcess
from pygecko import zmqTCP, zmqUDS
from pygecko import get_ip


ipAddress = get_ip()


def zmq_rq_rp(args):
    geckopy.init_node()

    # stop pub
    exit = Event()
    exit.clear()

    rmsg = {'a': 1, 'b': [1, 2, 3], 'c': 'hello cowboy'}
    msg = json.dumps(rmsg)
    msg = msg.encode("utf-8")

    def publisher(**kwargs):
        geckopy.init_node()
        exit = kwargs['exit']

        pt = kwargs["pub"]
        if pt == "bindtcp":
            p = Pub()
            p.bind(zmqTCP(ipAddress, 9100))
        elif pt == "connecttcp":
            p = Pub()
            p.connect(zmqTCP(ipAddress, 9101))
        elif pt == "binduds":
            p = Pub()
            p.bind(zmqUDS("/tmp/pygecko_test21"))
        elif pt == "connectuds":
            p = Pub()
            p.connect(zmqUDS("/tmp/pygecko_test22"))

        if p is None:
            assert False, "<<< Couldn't get Pub/Sub from geckocore >>>"

        rate = geckopy.Rate(3)

        for _ in range(100):
            if exit.is_set():
                break
            p.publish(msg)
            rate.sleep()

    p = GeckoSimpleProcess()
    args['exit'] = exit
    p.start(func=publisher, name='publisher', kwargs=args)

    st = args["sub"]

    if st == "connecttcp":
        s = Sub()
        s.connect(zmqTCP(ipAddress, 9100))
    elif st == "bindtcp":
        s = Sub()
        s.bind(zmqTCP(ipAddress, 9101))
    elif st == "connectuds":
        s = Sub()
        s.connect(zmqUDS("/tmp/pygecko_test21"))
    elif st == "binduds":
        s = Sub()
        s.bind(zmqUDS("/tmp/pygecko_test22"))

    rate = geckopy.Rate(2)

    for _ in range(5):
        m = s.recv_nb()
        mm = None
        if m:
            mm = json.loads(m)
            exit.set()
            break
        rate.sleep()

    p.join(0.01)
    assert rmsg == mm, "{} => {}".format(rmsg, mm)
