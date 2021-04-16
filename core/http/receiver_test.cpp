#include <iostream>

#include "receiver.hpp"

using namespace udp;


int main () {

    try {
        Receiver receiver;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}