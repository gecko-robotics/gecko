#!/usr/bin/env python3

import threading
import time
from mbeacon import BeaconCoreServer
from transport import Ascii


if __name__ == "__main__":

    bs = BeaconCoreServer(key='local',handler=Ascii)
    bs.start()
    bs.run()
