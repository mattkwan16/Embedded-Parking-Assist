#ifndef PING_H
#define PING_H

#include <string>

#define SPEED_OF_SOUND_MPS 343.0f

struct Ping {
    float amplitude;
    float tof;
    std::string key;
};

#endif