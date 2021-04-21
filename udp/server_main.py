#!/usr/bin/env python3


from udp import Server, get_logger
import cv2
import logging
import numpy as np


logger = get_logger(__name__)

def print_data(data):
    logger.info('Inside server callback:')
    logger.debug('Data:\n{}'.format(data))


def draw_frame(data):
    """
    data - a string representation of a numpy array, containing np.int8
    """

    arr = np.frombuffer(data, np.int8)

    # todo: replace hard-coded width/height value with values read from the received data
    # memory layout of received packet: | 0 - 1: height | 2 - 3: width | 4 - (height*width - 1): array values |
    arr = np.reshape(arr, (50, 20))
    cv2.imshow('test', arr)



server = Server(on_receive_callback=draw_frame)
server.listen()
