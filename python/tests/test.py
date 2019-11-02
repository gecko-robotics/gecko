import time
import os
from math import pi

from pygecko.multiprocessing import geckopy
from pygecko import FileJson, FileYaml
from pygecko import zmqTCP
from pygecko import zmqUDS

# from test_core import *
from test_no_core import *


# Files =================================================================
def file_func(Obj, fname):
    data = {'a': 1, 'bob': [1, 2, 3, 4], 'c': "hello cowboy", 'd': {'a': pi}}
    f = Obj()
    f.write(fname, data)
    d = f.read(fname)
    assert d == data, "{} != {}".format(d, data)
    os.remove(fname)


def test_json():
    file_func(FileJson, 'test.json')


def test_yaml():
    file_func(FileYaml, 'test.yml')


def test_rate():
    rate = geckopy.Rate(10)
    start = time.time()
    for _ in range(10):
        rate.sleep()
    stop = time.time()
    # print(stop - start)
    assert (stop - start) + 0.05 > 1.0


def test_endpoints():
    assert zmqUDS("/tmp/file") == "ipc:///tmp/file"
    assert zmqTCP("1.2.3.4", 12345) == "tcp://1.2.3.4:12345"
    assert zmqTCP("1.2.3.4") == "tcp://1.2.3.4"









#
# # Pub/Sub on TCP/UDS without Core ============================================
# def zmq_pub_sub(args):
#     geckopy.init_node()
#
#     # stop pub
#     exit = Event()
#     exit.clear()
#
#     rmsg = {'a': 1, 'b': [1, 2, 3], 'c': 'hello cowboy'}
#     msg = json.dumps(rmsg)
#     msg = msg.encode("utf-8")
#
#     def publisher(**kwargs):
#         geckopy.init_node()
#         exit = kwargs['exit']
#
#         pt = kwargs["pub"]
#         if pt == "bindtcp":
#             p = Pub()
#             p.bind(zmqTCP(ipAddress, 9000))
#         elif pt == "connecttcp":
#             p = Pub()
#             p.connect(zmqTCP(ipAddress, 9001))
#         elif pt == "binduds":
#             p = Pub()
#             p.bind(zmqUDS("/tmp/pygecko_test"))
#         elif pt == "connectuds":
#             p = Pub()
#             p.connect(zmqUDS("/tmp/pygecko_test2"))
#
#         if p is None:
#             assert False, "<<< Couldn't get Pub/Sub from geckocore >>>"
#
#         rate = geckopy.Rate(3)
#
#         for _ in range(100):
#             if exit.is_set():
#                 break
#             p.publish(msg)
#             rate.sleep()
#
#     p = GeckoSimpleProcess()
#     args['exit'] = exit
#     p.start(func=publisher, name='publisher', kwargs=args)
#
#     st = args["sub"]
#
#     if st == "connecttcp":
#         s = Sub()
#         s.connect(zmqTCP(ipAddress, 9000))
#     elif st == "bindtcp":
#         s = Sub()
#         s.bind(zmqTCP(ipAddress, 9001))
#     elif st == "connectuds":
#         s = Sub()
#         s.connect(zmqUDS("/tmp/pygecko_test"))
#     elif st == "binduds":
#         s = Sub()
#         s.bind(zmqUDS("/tmp/pygecko_test2"))
#
#     rate = geckopy.Rate(2)
#
#     for _ in range(5):
#         m = s.recv_nb()
#         mm = None
#         if m:
#             mm = json.loads(m)
#             exit.set()
#             break
#         rate.sleep()
#
#     assert rmsg == mm, "{} => {}".format(rmsg, mm)
#     p.join(0.01)
#
#
# def test_pub_bind_tcp():
#     args = {
#         'pub': 'bindtcp',
#         'sub': 'connecttcp'
#     }
#     zmq_pub_sub(args)
#
#
# def test_pub_connect_tcp():
#     args = {
#         'pub': 'connecttcp',
#         'sub': 'bindtcp'
#     }
#     zmq_pub_sub(args)
#
#
# def test_pub_bind_uds():
#     args = {
#         'pub': 'binduds',
#         'sub': 'connectuds'
#     }
#     zmq_pub_sub(args)
#
#
# def test_pub_connect_uds():
#     args = {
#         'key': 'test',
#         'topic': "test-uds-2",
#         'pub': 'connectuds',
#         'sub': 'binduds'
#     }
#     zmq_pub_sub(args)


# # Pub/Sub on TCP/UDS with Core ===============================================
# def zmq_pub_sub_core(args):
#     geckopy.init_node()
#
#     # stop pub
#     exit = Event()
#     exit.clear()
#
#     rmsg = {'a': 1, 'b': [1, 2, 3], 'c': 'hello cowboy'}
#     msg = json.dumps(rmsg)
#     msg = msg.encode("utf-8")
#
#     def publisher(**kwargs):
#         geckopy.init_node()
#         exit = kwargs['exit']
#
#         pt = kwargs["pub"]
#         key = kwargs["key"]
#         topic = kwargs["topic"]
#         if pt == "bindtcp":
#             p = geckopy.pubBinderTCP(key, topic)
#         elif pt == "connecttcp":
#             p = geckopy.pubConnectTCP(key, topic)
#         elif pt == "binduds":
#             p = geckopy.pubBinderUDS(key, topic, "/tmp/pygecko_test")
#         elif pt == "connectuds":
#             p = geckopy.pubConnectUDS(key, topic)
#
#         if p is None:
#             assert False, "<<< Couldn't get Pub from geckocore >>>"
#
#         rate = geckopy.Rate(3)
#
#         for _ in range(100):
#             if exit.is_set():
#                 break
#             p.publish(msg)
#             rate.sleep()
#
#     p = GeckoSimpleProcess()
#     args['exit'] = exit
#     p.start(func=publisher, name='publisher', kwargs=args)
#
#     time.sleep(1)
#
#     st = args["sub"]
#     key = args["key"]
#     topic = args["topic"]
#
#     if st == "connecttcp":
#         s = geckopy.subConnectTCP(key, topic)
#     elif st == "bindtcp":
#         s = geckopy.subBinderTCP(key, topic)
#     elif st == "connectuds":
#         s = geckopy.subConnectUDS(key, topic)
#     elif st == "binduds":
#         s = geckopy.subBinderUDS(key, topic, "/tmp/pygecko_test_2")
#
#     if s is None:
#         p.join(0.01)
#         assert False, "<<< Couldn't get Sub from geckocore: {} >>>".format(st)
#         return
#
#     rate = geckopy.Rate(2)
#
#     for _ in range(5):
#         m = s.recv_nb()
#         mm = None
#         if m:
#             mm = json.loads(m)
#             exit.set()
#             break
#         rate.sleep()
#
#     assert rmsg == mm, "{} => {}".format(rmsg, mm)
#     p.join(0.01)
#
#
# def test_pub_sub_core():
#
#     bs = CoreServer(key='test', handler=Ascii)
#     core = GeckoSimpleProcess()
#     core.start(func=bs.run, name='geckocore')
#
#     # time.sleep(2)
#
#     args = {
#         'key': 'test',
#         'topic': "test-tcp",
#         'pub': 'bindtcp',
#         'sub': 'connecttcp'
#     }
#     zmq_pub_sub_core(args)
#
#     args = {
#         'key': 'test',
#         'topic': "test-tcp-2",
#         'pub': 'connecttcp',
#         'sub': 'bindtcp'
#     }
#     zmq_pub_sub_core(args)
#
#     args = {
#         'key': 'test',
#         'topic': "test-uds",
#         'pub': 'binduds',
#         'sub': 'connectuds'
#     }
#     zmq_pub_sub_core(args)
#
#     args = {
#         'key': 'test',
#         'topic': "test-uds-2",
#         'pub': 'connectuds',
#         'sub': 'binduds'
#     }
#     zmq_pub_sub_core(args)
#
#     bs.stop()
#     # core.join(0.1)
#     core.terminate()



##########################################################################
#
# from collections import namedtuple
# from pygecko.messages.protocols import MsgPack
# from pygecko.messages.std_msgs import vec_t, quaternion_t, wrench_t, twist_t
# from pygecko.messages.nav_msgs import pose_t
# from pygecko.messages.sensor_msgs import imu_st, lidar_st, joystick_st, image_st
#
# # Fake cv2 things for testing
# import pygecko.fake.fake_camera as pcv2
#
#
# def test_messages():
#     buffer = MsgPack()
#
#     im = pcv2.cvImage(20, 20)  # fake image
#
#     tests = [
#         22,
#         "hello world",
#         (1, 2, 3),
#         # [1,2,3,4],  # lists always => tuples
#         {'a': 1, 'b': 3},
#         vec_t(1, 2, 3),
#         quaternion_t(1, 2, 3, 4),
#         wrench_t(vec_t(1, 2, 3), vec_t(4, 5, 6)),
#         pose_t(vec_t(1, 2, 3), quaternion_t(4, 5, 6, 7)),
#         twist_t(vec_t(1, 2, 3), vec_t(4, 5, 6)),
#         imu_st(vec_t(1, 2, 3), vec_t(4, 5, 6), vec_t(7, 8, 9)),
#         lidar_st(((1, 2), (3, 4), (5, 6), (7, 8))),
#         joystick_st((1, 2, 3), (0, 0, 0, 1), "ps4"),
#         image_st(im.shape, im.tobytes(), False)
#     ]
#
#     for t in tests:
#         m = buffer.pack(t)
#         m = buffer.unpack(m)
#         assert t == m, "{} => {}".format(t, m)
#
#
# def test_new_messages():
#     """
#     Add new messages to pygecko
#     """
#     class msg_t(namedtuple('msg_t', 'x y z')):
#         __slots__ = ()
#
#         def __new__(cls, x, y, z):
#             cls.id = 111
#             return cls.__bases__[0].__new__(cls, x, y, z)
#
#     def unpack(id, data):
#         # unpack new message with id 111
#         if id == 111:
#             return msg_t(*data)
#         return None
#
#     # buffer = MsgPack(list(IntFlag('new', {'msg_t': 111})), unpack)
#     buffer = MsgPack([111], unpack)
#     m = msg_t(1, 2, 3)
#     b = buffer.pack(m)
#     b = buffer.unpack(b)
#     assert m == b, "{} => {}".format(m, b)

#
# def msg_zmq(args):
#     # start message hub
#     # core = GeckoCore()
#     # core.start()
#     bs = CoreServer(key='test', handler=Ascii)
#     bs.start()
#     bs.run()
#
#     msg1 = imu_st(
#         vec_t(1, 2, 3),
#         vec_t(11, 12, 13),
#         vec_t(21, 22, 23))
#
#     msg2 = twist_t(
#         vec_t(1, 2, 3),
#         vec_t(11, 12, 13))
#
#     msg3 = pose_t(
#         vec_t(1, 2, 3),
#         quaternion_t(1, 2, 3, 4))
#
#     msg4 = lidar_st(
#         (
#             (1, 1),
#             (2, 2),
#             (3, 3)
#         )
#     )
#
#     def publisher(**kwargs):
#         geckopy.init_node(**kwargs)
#         # p = geckopy.Publisher(topics=['test'])
#         # uds = kwargs.get('path')
#         p = Pub()
#         p.bind(kwargs.get('path'))
#         time.sleep(1)  # need this!!
#
#         for msg in [msg1, msg2, msg3, msg4]:
#             # for msg in [msg1, msg2]:
#             p.publish(msg)
#             time.sleep(0.01)
#
#     p = GeckoSimpleProcess()
#     p.start(func=publisher, name='publisher', kwargs=args)
#
#     # subscriber
#     s = Sub()
#     s.topics = args.get('topics')
#     s.connect(args.get('path'))
#
#     for msg in [msg1, msg2, msg3, msg4]:
#         m = s.recv()
#         # assert t == b'test'  # FIXME: fix stupid binary string crap!
#         assert msg == m
#
#     # core.join(0.1)
#     time.sleep(1)  # if I run these too fast, I get errors on bind()

#
# # def test_msg_zmq_tcp():
# #     args = {
# #         'path': zmqTCP('localhost', 9999),
# #         'topics': 'bob'
# #     }
# #     msg_zmq(args)
# #
# #
# # def test_msg_zmq_uds():
# #     args = {
# #         'path': zmqUDS('/tmp/udstest'),
# #         'topics': 'bob'
# #     }
# #     msg_zmq(args)
#
# # bs = BeaconCoreServer(key='test', handler=Ascii)
# # core = GeckoSimpleProcess()
# # core.start(func=bs.run, name='geckocore')
#
