##############################################
# The MIT License (MIT)
# Copyright (c) 2014 Kevin Walchko
# see LICENSE for full details
##############################################
#
# Kevin J. Walchko 13 Oct 2014
#
# see http://zeromq.org for more info
# http://zguide.zeromq.org/py:all
import zmq
# import time
from pygecko.transport.zmq_base import Base


class Reply(Base):
    """
    Reply?
    """
    def __init__(self):
        Base.__init__(self, zmq.REP)

    def __del__(self):
        """
        wtf! I need this or it hangs
        """
        self.close()

    def listen(self, callback, flags=0):
        """
        checks to see if a request needs to be serviced
        flags: blocking or non-blocking using zmq.NOBLOCK
        callback: a function to handle a request message and return the answer.
            The function must be able to handle a message it didn't expect.
            Since there are no topics associated with this, a user might send
            some crazy message

            callback(message) -> answer

        returns True if serviced, False if not
        """
        ret = False
        msg = self.recv_poll()
        if msg:
            msg = callback(msg)
            self.socket.send(msg)
            ret = True

        return ret

    # def listen_nb(self, callback, flags=0):
    #     """
    #     see listen()
    #     """
    #     return self.listen(callback=callback, flags=zmq.NOBLOCK)


class Request(Base):
    """
    Request
    """
    def __init__(self):
        Base.__init__(self, zmq.REQ)

    def __del__(self):
        """Calls Base.close()"""
        self.close()

    def get(self, msg, flags=0):
        """
        Implements recv_multipart(flags) with flags=0 for blocking.
        msg: request message sent to a reply class
        rmsg: the returned message from a reply class, None if no message in
              non-blocking mode
        flags: blocking or non-blocking, using zmq.NOBLOCK or zmq.DONTWAIT
        """
        rmsg = None  # returned message
        self.socket.send(msg)
        rmsg = self.recv_poll()
        return rmsg
