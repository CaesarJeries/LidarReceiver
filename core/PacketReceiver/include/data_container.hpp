#ifndef __DATA_CONTAINER_HPP__
#define __DATA_CONTAINER_HPP__

#include <array>
#include <hash_map>

#include "lidar.hpp"
#include "lidar_data.hpp"

namespace caesar {

namespace lidar_emulator {

class FrameData {
    /**
    `frame`: sensor readings 
    `number_received`: the number of received measurements in the point cloud
    */

    std::array<Point, POINT_CLOUD_SIZE> frame;
    ulong number_received;

    public:
    
    FrameData();
    ~FrameData() = default;
};

class LidarDataContainer {

    std::hash_map<index_t, FrameData> m_frames;

    public:
    LidarDataContainer() = default;
    ~LidarDataContainer() = default;

    void put(index_t frame_number, FrameData&& frame_data);
    void remove(index_t frame_number);

    // is ready for display, i.e, all points in the point cloud were received
    bool is_ready(index_t frame_number) const;

    LidarDataContainer(const LidarDataContainer&) = delete;
    LidarDataContainer& operator=(const LidarDataContainer&) = delete;

    LidarDataContainer(LidarDataContainer&&) = default;
    LidarDataContainer& operator=(LidarDataContainer&&) = default;

}; // class LidarDataContainer

}

}


#endif // __DATA_CONTAINER_HPP__
