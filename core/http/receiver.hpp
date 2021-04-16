#ifndef __UDP_RECEIVER_HPP__
#define __UDP_RECEIVER_HPP__

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <stdexcept>
#include <errno.h>
#include <unistd.h> // ::close
#include <iostream>

namespace udp {

class Receiver {

    class FailedToCreateSocketException: public std::runtime_error {
        public:
        explicit FailedToCreateSocketException(const std::string& msg) : std::runtime_error(msg)
        {}
    };

    using socket_t = int;
    using socket_address_t = sockaddr_in;

    private:

    socket_t m_socket;
    socket_address_t m_socket_address;

    public:

    explicit Receiver(std::string ip = "127.0.0.1", int port = 4242) {
        m_socket = socket(AF_INET, SOCK_DGRAM, 0);
        m_socket_address.sin_port = htons(port);
        m_socket_address.sin_family = AF_INET;
        
        if (inet_pton(AF_INET, ip.c_str(), &m_socket_address.sin_addr) < 0) {
            // Address conversion failed
            throw FailedToCreateSocketException(std::string("Failed to convert ip address: ") + strerror(errno));
        }

        std::clog << "Trying to bind socket to address: " << ip << ", port: " << port << std::endl;
        if (bind(m_socket, (sockaddr*)&m_socket_address, sizeof(m_socket_address)) < 0) {
            throw FailedToCreateSocketException(std::string("Failed to bind address to socket: ") + strerror(errno));
        }

        std::clog << "Socket created successfully\n";
    }

    ~Receiver() {
        std::clog << "Closing socket\n";
        ::close(m_socket);
    }

};

};

#endif // __UDP_RECEIVER_HPP__
