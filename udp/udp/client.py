import logging
import socket
import sys



class Client(object):

    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
        logging.info('Created a socket for client')

    def send(self, data, ip = 'localhost', port = 4242):
        # send data through UDP protocol
        logging.info('Sending data: {} to (ip, port): ({}, {})'.format(data, ip, port))
        self.socket.sendto(data.encode('utf-8'), (ip, port))
        
        data, address = self.socket.recvfrom(port)
        logging.info('\nClient received from {}: '.format(address), data.decode('utf-8'), '\n')
    
    def __del__(self):
        # close the socket
        logging.info('Closing socket')
        self.socket.close()
