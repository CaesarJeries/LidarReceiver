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

    /**
    A receiver is a udp server that listens to incoming packets.
    */

    class FailedToCreateSocketException: public std::runtime_error {
        public:
        explicit FailedToCreateSocketException(const std::string& msg) : std::runtime_error(msg)
        {}
    };

    class TransmissionFailureException:  public std::runtime_error {
        public:
        explicit TransmissionFailureException(const std::string& msg) : std::runtime_error(msg)
        {}
    };


    using socket_t = int;
    using socket_address_t = sockaddr_in;

    private:

    socket_t m_socket;
    socket_address_t m_socket_address;
    std::vector<byte_t> m_buffer;

    public:

    // todo: replace with a shared size definition relative to the current definition
    // of UdpPacket, width/height of the sample size
    const int MAX_PAYLOAD_SIZE = 1000;
    const int PAYLOAD_HEADER_SIZE = 12; // frameno, start_index, end_index

    explicit Receiver(std::string ip = "127.0.0.1", int port = 4242) : m_buffer(MAX_PAYLOAD_SIZE) {
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

    /**
    Reads the packet header, determines the data buffer size, and then receives the data buffer
    */
    void listen() {
        ssize_t received = recvfrom(m_socket, &m_buffer[0], PAYLOAD_HEADER_SIZE, MSG_WAITALL, nullptr, nullptr);
        if (received < 0) {
            std::cerr << std::string("Error encountered while receiving packet header: ") + strerror(errno) << std::endl;
            return;
        }
        if (received < PAYLOAD_HEADER_SIZE) {
            throw TransmissionFailureException("Error encountered while receiving packet header: Not all bytes received");
        }

        // todo: is the cast safe? is there an alternative?
        int frameno = *reinterpret_cast<int*>(&m_buffer[0]);
        int start_index = *reinterpret_cast<int*>(&m_buffer[4]);
        int end_index = *reinterpret_cast<int*>(&m_buffer[8]);

        size_t total_data_size = end_index - start_index;
        received = recvfrom(m_socket, &m_buffer[0], total_data_size, MSG_WAITALL, nullptr, nullptr);
        if (received < 0) {
            throw TransmissionFailureException(std::string("Error encountered while receiving packet data: ") + strerror(errno));
        }
        if (received < total_data_size) {
            throw TransmissionFailureException("Error encountered while receiving packet data: Not all bytes received");
        }
    }

    ~Receiver() {
        std::clog << "Closing socket\n";
        ::close(m_socket);
    }

};

};

#endif // __UDP_RECEIVER_HPP__
