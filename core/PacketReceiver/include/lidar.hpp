#ifndef __LIDAR_HPP__
#define __LIDAR_HPP__


namespace caesar {

namespace lidar_simulator {

namespace point_cloud {

constexpr int HEIGHT = 32;
constexpr int WIDTH = 32;
constexpr int SIZE = HEIGHT * WIDTH; // 1024

}

constexpr int FRAME_RATE = 15/*fps*/; 

} // namespace caesar

} // namespace lidar_emulator

#endif // __LIDAR_HPP__
