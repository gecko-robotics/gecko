#!/usr/bin/env python

"""
Publishers

app -t hostname port -u file -c key -j file

pre-determined endpoint: -t or -u
automate (multicast): -c
other settings: -j
"""


import argparse

help_description = """

"""


def handleArgs():
    parser = argparse.ArgumentParser(description=help_description)

    group = parser.add_argument_group('connection')

    feature_parser = group.add_mutually_exclusive_group(required=True)

    feature_parser.add_argument('-j', '--json', help="use json file")
    feature_parser.add_argument('-t', '--tcp', nargs=2, help="enable TCP communication: host port")
    feature_parser.add_argument('-u', '--uds', help="enable UDS communication: file")
    feature_parser.add_argument('-c', '--core', nargs=2, help="enable core communication: key topic")

    group.set_defaults(core=True)

    # parser.add_argument("-T", "--Topic", help="topic name")
    # parser.add_argument('--mode', choices=('read-only', 'read-write'))

    args = vars(parser.parse_args())
    return args


if __name__ == "__main__":
    args = handleArgs()
    print(args)
