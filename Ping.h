#ifndef PING_H
#define PING_H

#include <string>
#include <limits>

#define SPEED_OF_SOUND_MPS 343.0f

struct Ping {
    float amplitude;
    float tof;
    std::string key;
    Ping () :
        amplitude(0.0f),
        tof(std::numeric_limits<float>::max()),
        key("") {}
};

#endif