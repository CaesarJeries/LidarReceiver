import logging
import socket
import sys
from .common import get_logger

logger = get_logger(__name__)

class Server(object):
    def __init__(self, ip = 'localhost', port = 4242, on_receive_callback=None):
        # Create socket for server
        logger.info('Creating a socket for server (ip, port): ({}, {})'.format(ip, port))
        self.ip = ip
        self.port = port
        server_address = (ip, port)
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
        logger.debug('Socket created')
        logger.debug('Binding socket to address')
        self.socket.bind(server_address)
        self.callback = on_receive_callback

    def listen(self):
        while True:
            logger.info('Listening on port: {}'.format(self.port))
            data, address = self.socket.recvfrom(1024) # todo: replace hard-coded size
            logger.info('Received data from address: {}'.format(address))
            logger.debug('Data\n:')
            logger.debug(data)

            if self.callback is not None:
                self.callback(data)
    
    def __del__(self):
        # close the socket
        logger.info('Closing socket (ip, port): ({}, {})'.format(self.ip, self.port))
        self.socket.close()

