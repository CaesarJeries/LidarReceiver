#ifndef __LIDAR_DATA_HPP__
#define __LIDAR_DATA_HPP__

#include "common.hpp"

namespace caesar {

namespace lidar_emulator {

struct Point {
    float x, y, z; // 3D coordinates. Can be any value
    float intensity; // 0 - 1 (inclusive)
};

struct Packet {
    index_t frame_number;
    index_t start_index;
    index_t end_index; // size = end_index - start_index + 1
    Point* data;
};


} // namespace caesar

} // namespace lidar_emulator

#endif // __LIDAR_DATA_HPP__
