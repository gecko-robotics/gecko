import time
from multiprocessing import Event
import json

from pygecko.apps.core import CoreServer
from pygecko.network.transport import Ascii
from pygecko.multiprocessing import geckopy
from pygecko.multiprocessing.process import GeckoSimpleProcess
from pygecko import get_ip


ipAddress = get_ip()


# Pub/Sub on TCP/UDS with Core ===============================================
def zmq_pub_sub_core(args):
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
        key = kwargs["key"]
        topic = kwargs["topic"]
        if pt == "bindtcp":
            p = geckopy.pubBinderTCP(key, topic)
        elif pt == "connecttcp":
            p = geckopy.pubConnectTCP(key, topic)
        elif pt == "binduds":
            p = geckopy.pubBinderUDS(key, topic, "/tmp/pygecko_test")
        elif pt == "connectuds":
            p = geckopy.pubConnectUDS(key, topic)

        if p is None:
            assert False, "<<< Couldn't get Pub from geckocore >>>"

        rate = geckopy.Rate(3)

        for _ in range(100):
            if exit.is_set():
                break
            p.publish(msg)
            rate.sleep()

    p = GeckoSimpleProcess()
    args['exit'] = exit
    p.start(func=publisher, name='publisher', kwargs=args)

    time.sleep(1)

    st = args["sub"]
    key = args["key"]
    topic = args["topic"]

    if st == "connecttcp":
        s = geckopy.subConnectTCP(key, topic)
    elif st == "bindtcp":
        s = geckopy.subBinderTCP(key, topic)
    elif st == "connectuds":
        s = geckopy.subConnectUDS(key, topic)
    elif st == "binduds":
        s = geckopy.subBinderUDS(key, topic, "/tmp/pygecko_test_2")

    if s is None:
        p.join(0.01)
        assert False, "<<< Couldn't get Sub from geckocore: {} >>>".format(st)
        return

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
    print("process:", p)
    assert rmsg == mm, "{} => {}".format(rmsg, mm)


def CoreProc(**kwargs):
    bs = CoreServer(key='test', handler=Ascii)
    # bs.start()
    # bs.run()

    exit = kwargs['exit']
    while not exit.is_set():
        bs.loopOnce()


def test_pub_sub_core():
    print("==============================")
    print("| Starting Core Tests        |")
    print("==============================")

    # bs = CoreServer(key='test', handler=Ascii)

    # stop pub
    exit = Event()
    exit.clear()
    args = {}
    args['exit'] = exit

    core = GeckoSimpleProcess()
    core.start(func=CoreProc, name='geckocore', kwargs=args)

    # time.sleep(2)

    args = {
        'key': 'test',
        'topic': "test-tcp",
        'pub': 'bindtcp',
        'sub': 'connecttcp'
    }
    zmq_pub_sub_core(args)

    args = {
        'key': 'test',
        'topic': "test-tcp-2",
        'pub': 'connecttcp',
        'sub': 'bindtcp'
    }
    zmq_pub_sub_core(args)

    args = {
        'key': 'test',
        'topic': "test-uds",
        'pub': 'binduds',
        'sub': 'connectuds'
    }
    zmq_pub_sub_core(args)

    args = {
        'key': 'test',
        'topic': "test-uds-2",
        'pub': 'connectuds',
        'sub': 'binduds'
    }
    zmq_pub_sub_core(args)

    exit.set()
    core.terminate()
