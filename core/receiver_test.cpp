#include <iostream>

#include "receiver.hpp"

using namespace caesar;


/**
todo:
create a listener thread.
listen for incoming packets.
on packet arrival:
insert to data structure
if all chunks for a certain frame were received, draw frame, cleanup data.

todo:
implement a mechanism for clean exit
**/
int main () {

    try {
        Receiver receiver;

        receiver.listen();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}