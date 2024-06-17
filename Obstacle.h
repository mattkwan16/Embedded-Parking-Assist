#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Ping.h"

struct Obstacle {
    float x;
    float y;

    Obstacle(float x, float y);

    Ping ping(const Ping &p) const;  // Method to handle a ping
};

#endif // OBSTACLE_H
