from enum import IntFlag

GeckoMsgFlags = IntFlag(
    'GeckoMsgFlags',
    {
        'vector':     0,
        'quaternion': 1,
        'wrench':     2,
        'pose':       3,
        'twist':      4,
        'imu':       10,
        'joystick':  11,
        'image':     12,
        'lidar':     20
    }
)

GeckoMsgs = list(GeckoMsgFlags)
