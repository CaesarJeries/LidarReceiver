import logging
import socket
import sys


logging.basicConfig(level=logging.DEBUG, format='[%(filename)s:%(module)s:%(funcName)s:%(lineno)d] - %(message)s')

class Server(object):
    def __init__(self, ip = 'localhost', port = 4242):
        # Create socket for server
        logging.debug('Creating a socket for server (ip, port): ({}, {})'.format(ip, port))
        self.ip = ip
        self.port = port
        server_address = (ip, port)
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
        logging.debug('Socket created')
        logging.debug('Binding socket to address')
        self.socket.bind(server_address)

    def listen(self):
        while True:
            logging.info('Listening on port: {}'.format(self.port))
            data, address = self.socket.recvfrom(self.port)
            logging.info('Received data from address: {}'.format(address))
            logging.info('Data\n:')
            logging.info(data)
    
    def __del__(self):
        # close the socket
        logging.info('Closing socket (ip, port): ({}, {})'.format(self.ip, self.port))
        self.socket.close()

