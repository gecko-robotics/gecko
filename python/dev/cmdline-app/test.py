#!/usr/bin/env python

import argparse

def handleArgs():
    parser = argparse.ArgumentParser(description='template')

    feature_parser = parser.add_mutually_exclusive_group(required=False)
    # feature_parser.add_argument('-T', '--Tcp', dest='useTcp', action='store_true', help="enable TCP communication [default]")
    feature_parser.add_argument('-t', '--tcp', dest='useTcp', help="enable TCP communication [default]")
    feature_parser.add_argument('-u', '--uds', dest='useUds', help="enable UDS communication")
    feature_parser.add_argument('-c', '--core', dest='useCore', help="enable core communication")
    parser.set_defaults(useTcp=True)

    # parser.add_argument('-p', '--publish', nargs=2, help='publish messages to addr:port, ex. js 10.1.1.1 9000', default=['localhost', '9000'])
    # parser.add_argument('key', help='geckocore machine key')
    # parser.add_argument('-f', '--file', help='UDS file', default='/tmp/twist_kb')
    # parser.add_argument('-v', '--verbose', help='display info to screen', action='store_true')
    args = vars(parser.parse_args())
    return args

if __name__ == "__main__":
    args = handleArgs()
    print(args)
