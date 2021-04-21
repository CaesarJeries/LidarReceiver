import argparse
from udp import Client
import sys


def get_argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', action='store_const', default='localhost')
    parser.add_argument(['--port', '-p'], action='store_const', default=4242)

    return parser


def main():
    parser = get_argument_parser()
    args = parser.parse_args()
    client = Client(ip = args.ip, port = args.port)

    while True:
        data = input()
        if 'quit' == data:
            break
        client.send(data)

if __name__ == '__main__':
    main()
