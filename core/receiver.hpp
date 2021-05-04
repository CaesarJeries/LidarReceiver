#ifndef __UDP_RECEIVER_HPP__
#define __UDP_RECEIVER_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <errno.h>
#include <unistd.h> // ::close
#include <iostream>
#include <vector>

namespace caesar {

class Receiver {

    /**
    A receiver is a udp server that listens to incoming packets.
    */

    public:

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
    using byte_t = unsigned char;

    // create an endpoint that listens on ip:port for incoming traffic
    explicit Receiver(std::string ip = "127.0.0.1", int port = 4242);

    /**
    Reads the packet header, determines the data buffer size, and then receives the data buffer
    */
    void listen();

    ~Receiver();

    private:

    const int MAX_PAYLOAD_SIZE = 1024;
    const int PAYLOAD_HEADER_SIZE = 12; // frameno, start_index, end_index. 4 bytes each
    
    socket_t m_socket;
    socket_address_t m_socket_address;
    std::vector<byte_t> m_buffer;
};

};

#endif // __UDP_RECEIVER_HPP__
