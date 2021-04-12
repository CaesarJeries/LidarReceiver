#ifndef __PACKET_RECEIVER_HPP__
#define __PACKET_RECEIVER_HPP__

#include <string>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

using std::clog;
using std::endl;
using std::string;





class PacketReceiver {

    private:

    std::string ip;
    uint port;

    public:

    using socket_t = struct sockaddr_in; 

    public:
    PacketReceiver(std::string ip="localhost", uint port);
    ~PacketReceiver();

    PacketReceiver(const PacketReceiver&) = delete;
    PacketReceiver& operator=(const PacketReceiver&) = delete;


    PacketReceiver(PacketReceiver&& src) = delete;
    PacketReceiver& operator=(PacketReceiver&& src) = delete;

}; // PacketReceiver



#endif // __PACKET_RECEIVER_HPP__
