#!/usr/bin/env python

from multiprocessing import Event
import json
import time

# from pygecko.multiprocessing import geckopy
# from pygecko.transport.zmq_sub_pub import Pub, Sub
from pygecko.transport.zmq_req_rep import Reply, Request
from pygecko.multiprocessing.process import GeckoSimpleProcess
from pygecko import zmqTCP, zmqUDS
from pygecko import get_ip
import shutil          # move and delete files/folders
import os

def rmdir(path):
    try:
        shutil.rmtree(path)
    except FileNotFoundError:
        # folder was already deleted or doesn't exist ... it's ok
        pass

ipAddress = get_ip()

endpt = zmqUDS("/tmp/req_rpy_0")
if os.path.exists(endpt):
    os.remove(endpt)

def proc():
    def cb(msg):
        print("cb send:", msg)
        msg = json.dumps({"recv send": "ans"}).encode("utf-8")
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
    time.sleep(5)
    msg = json.dumps({"request send": "question"}).encode("utf-8")
    ans = req.get(msg)
    print(">> request good?", ans)

    time.sleep(0.01)
