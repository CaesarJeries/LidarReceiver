#!/usr/bin/env python3

import logging
from udp import Client

logging.basicConfig(format='%(message)s')

client = Client()

client.send('Hello world!')
