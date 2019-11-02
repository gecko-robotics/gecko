# :lizard:  pyGecko

[![Latest Version](https://img.shields.io/pypi/v/pygecko.svg)](https://pypi.python.org/pypi/pygecko/)
[![License](https://img.shields.io/pypi/l/pygecko.svg)](https://pypi.python.org/pypi/pygecko/)
[![image](https://img.shields.io/pypi/pyversions/pygecko.svg)](https://pypi.python.org/pypi/pygecko)
[![image](https://img.shields.io/pypi/format/pygecko.svg)](https://pypi.python.org/pypi/pygecko)
[![Build Status](https://travis-ci.org/gecko-robotics/pygecko.svg?branch=master)](https://travis-ci.org/gecko-robotics/pygecko)


## Install

- From pypi: `pip install pygecko`
- Local for development: `pip install -e .`

## `geckolaunch.py`

`geckolaunch.py` allows you to launch a bunch of processes quickly using a launch
file. A launch file is just a simple json file where each line takes the form:
`[file, function, kwargs]`. Here is an example:

```bash
{
  "processes":
  [
    ["process", "publish", {"topic": "hello"}],
    ["process", "publish", {"topic": "hey there"}],
    ["process", "subscribe2", {"topic": "hello"}],
    ["process", "subscribe2", {"topic": "hello"}],
    ["process", "subscribe2", {"topic": "hey there"}],
    ["process", "subscribe2", {"topic": "hey there"}],
    ["process", "subscribe2", {"topic": "cv"}],
    ["process", "subscribe2", {"topic": "cv"}],
    ["process", "pcv", {"topic": "cv"}]
  ],
  "geckocore": {
      "host": "localhost"  # or hostname.local
  }
}
```

# Basic User API

## `geckopy`

See the examples, but this acts like a `rospy` and helps make writing
pub/sub processes easy. See the `/examples` folder to see it in action.

- **init_node():** this sets up the the process for communications with `geckocore`
- **is_shutdown():** returns `True` if node is signalled to exit
- **ok():** returns `True` if node is running
- **getLogger(name, file):** prints log messages to `stdout` or file
    ```python
    from pygecko import geckopy
    logger = geckopy.getLogger(__name__, "bob.txt")
    logger.info('this is a info message')  # just send a string
    logger.warning('this is a warning message')
    logger.error('this is a error message')
    logger.critical('this is a critical message')
    logger.debug('this is a debug message')
    ```
- **Rate(Hz):** given a frequency of how often a loop should run (i.e., 10Hz), the
returned object will dynamically set the sleep interval to achieve the rate. Ex:
    ```python
    from pygecko import geckopy
    rate = geckopy.Rate(20)  # run loop at 20 Hz
    while True:
        rate.sleep()
    ```
- **Publisher Helpers:** return a publisher object, or `None` on failure
    - pubBinderTCP(key, topic, queue_size=5)
    - pubBinderUDS(key, topic, queue_size=5)
    - pubConnectTCP(key, topic, queue_size=5)
    - pubConnectUDS(key, topic, queue_size=5)
- **Subscriber Helpers:** return a subscriber object, or `None` on failure
    - subBinderTCP(key, topic, queue_size=5)
    - subBinderUDS(key, topic, queue_size=5)
    - subConnectTCP(key, topic, queue_size=5)
    - subConnectUDS(key, topic, queue_size=5)

## `zmq`

Generally you will use the `geckopy` helpers to get pub/sub. However, if you
already know the address and don't need core, you can create one directly. This
is most useful for testing.

- **Subscriber:** creates a subscriber and returns it
    - Sub.connect(self, addr, hwm=None, queue_size=10)
    - Sub.bind(self, addr, hwm=None, queue_size=10, random=False)
- **Publisher:** creates a publisher and returns it
    - Pub.connect(self, addr, hwm=None, queue_size=10)
    - Pub.bind(self, addr, hwm=None, queue_size=10, random=False)

# Change Log

Date        |Version| Notes
------------|-------|---------------------------------
2019-May-18 | 1.3.0 | working with c++
2019-Mar-02 | 1.2.0 | set multicast as the default method to find nodes
2018-Oct-28 | 1.1.0 | simplified and removed geckocore as the main hub
2018-Sep-16 | 1.0.3 | implemented a multicast connection process
2018-Sep-16 | 1.0.2 | dropping python 2.7 support, only 3.7+
2018-Sep-11 | 1.0.1 | working, but still need to flush it out some more
2018-Jul-28 | 1.0.0 | totally nuked everything from orbit and started over
2017-May-14 | 0.8.3 | updates and refactor
2017-Apr-02 | 0.8.2 | fix pypi doc and refactor
2017-Mar-19 | 0.7.0 | refactored
2017-Mar-12 | 0.6.0 | changed messages from dict to classes
2016-Dec-26 | 0.5.0 | refactor
2016-Oct-09 | 0.4.1 | published to PyPi
2010-Mar-10 | 0.0.1 | init


# MIT License

**Copyright (c) 2010 Kevin J. Walchko**

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
