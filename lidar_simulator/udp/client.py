import logging
import socket
import sys
from .common import get_logger

logger = get_logger(__name__)

class Client(object):

    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
        logger.info('Created a socket for client')

    def send(self, data, ip = 'localhost', port = 4242):
        """
        Sends the given data to the specified ip:port.
        No data encoding is performed. The data is sent as is.
        """

        # send data through UDP protocol
        logger.info('Sending data: to (ip, port): ({}, {})'.format(ip, port))
        logger.debug(data)
        self.socket.sendto(data, (ip, port))
        
        #data, address = self.socket.recvfrom(port)
        #logger.info('\nClient received from {}: '.format(address), data.decode('utf-8'), '\n')
    
    def __del__(self):
        # close the socket
        logger.info('Closing socket')
        self.socket.close()
