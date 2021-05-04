#include "receiver.hpp"

namespace caesar {


Receiver::Receiver(std::string ip, int port) : m_buffer(MAX_PAYLOAD_SIZE) {
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


Receiver::~Receiver() {
    std::clog << "Closing socket\n";
    ::close(m_socket);
}


void Receiver::listen() {
loop:
    ssize_t received = recvfrom(m_socket, &m_buffer[0], PAYLOAD_HEADER_SIZE, MSG_WAITALL, nullptr, nullptr);
    if (received < 0) {
        std::cerr << std::string("Error encountered while receiving packet header: ") + strerror(errno) << std::endl;
        return;
    }
    if (received < PAYLOAD_HEADER_SIZE) {
        throw TransmissionFailureException("Error encountered while receiving packet header: Not all bytes received");
    }

    // todo: is the cast safe? is there an alternative?
    int frameno = ntohl(*reinterpret_cast<int*>(&m_buffer[0]));
    int start_index = *reinterpret_cast<int*>(&m_buffer[sizeof(int)]);
    int end_index = *reinterpret_cast<int*>(&m_buffer[2*sizeof(int)]);

    std::clog << "Received values: frameno = " << frameno << \
                 " , start_index = " << start_index << \
                 ", end_index = " << end_index << '\n'; 

    size_t total_data_size = end_index - start_index;
    received = recvfrom(m_socket, &m_buffer[0], total_data_size, MSG_WAITALL, nullptr, nullptr);
    if (received < 0) {
        throw TransmissionFailureException(std::string("Error encountered while receiving packet data: ") + strerror(errno));
    }
    if (received < total_data_size) {
        throw TransmissionFailureException("Error encountered while receiving packet data: Not all bytes received");
    }

    goto loop;

}

}
