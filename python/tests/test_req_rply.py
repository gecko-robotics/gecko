from multiprocessing import Event
import json
import time
import os

from pygecko.multiprocessing import geckopy
from pygecko.transport.zmq_sub_pub import Pub, Sub
from pygecko.transport.zmq_req_rep import Reply, Request
from pygecko.multiprocessing.process import GeckoSimpleProcess
from pygecko import zmqTCP, zmqUDS
from pygecko import get_ip

ipAddress = get_ip()


endpt = zmqUDS("/tmp/req_rpy_0")

def clean():
    if os.path.exists(endpt):
        os.remove(endpt)

def test_s():
    clean()

    orig_ans = {"recv send": "ans"}

    def proc():
        def cb(msg):
            print("cb send:", msg)
            msg = json.dumps(orig_ans).encode("utf-8")
            print("cb recv:", msg)
            return msg

        rep = Reply()
        rep.bind(endpt)
        good = False

        while not good:
            good = rep.listen(cb)
            print(">> reply good?", good)

    p = GeckoSimpleProcess()
    p.start(func=proc, name='reply')


    req = Request()
    req.connect(endpt)

    ans = None
    while ans is None:
        time.sleep(3)
        msg = json.dumps({"request send": "question"}).encode("utf-8")
        ans = req.get(msg)

        if ans:
            ans = json.loads(ans)
            print(">> request good?", ans)

        time.sleep(0.01)

    assert orig_ans == ans, "{} != {}".format(orig_ans, ans)
