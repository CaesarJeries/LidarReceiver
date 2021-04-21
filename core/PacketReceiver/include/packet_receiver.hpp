#ifndef __PACKET_RECEIVER_HPP__
#define __PACKET_RECEIVER_HPP__

#include <string>
#include<stdio.h>
#include<stdlib.h>
#include <memory>
#include <hash_map>

#include "http/sender.hpp"
#include "http/receiver.hpp"

using std::clog;
using std::endl;
using std::string;





class PacketReceiver {
    
    public:

    using byte_t = unsigned char;
    using ulong_t = unsigned long;
    using index_t = ulong;
    using string_t = std::string;
    using hash_map_t = std::hash_map; // todo: replace with your own impl


    struct quality {

        size_t constexpr size() const {
            return height * width;
        }

    };

    struct 480p: public quality {
        // used for testing with cv: lower quality without sacrificing accuracy
        const int height = 640;
        const int width = 480;
        const int fps 15;
    };

    struct 720p: public quality {
        const int height = 1280;
        const int width = 720;
        const int fps = 30;
    };

    struct 1080p: public quality {
        const int height = 1920;
        const int width = 1080;
        const int fps = 30;
    };

    template<typename Quality>
    using data_array_t = byte_t[Quality::size()];

    struct Packet {
        ulong_t frame_number;
        std::unique_ptr<data_array_t> data;
        index_t start_index;
        index_t end_index;

    }; // Packet


    private:

    string_t ip;
    uint port;
    http::receiver receiver;


    public:
    PacketReceiver(std::string ip="localhost");
    ~PacketReceiver();

    PacketReceiver(const PacketReceiver&) = delete;
    PacketReceiver& operator=(const PacketReceiver&) = delete;

    PacketReceiver(PacketReceiver&& src) = delete;
    PacketReceiver& operator=(PacketReceiver&& src) = delete;

}; // PacketReceiver



#endif // __PACKET_RECEIVER_HPP__
