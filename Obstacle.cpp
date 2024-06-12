#include "Obstacle.h"
#include <iostream>
#include <cmath>

#define RANGE 10.0f
#define SCALE 1.0f 

Obstacle::Obstacle(float x, float y, float sz) : x(x), y(y), size(sz) {}

float Obstacle::ping() const {
    // Calculate the distance from the origin (0, 0)
    float distance = std::sqrt(x * x + y * y);

    if (distance <= RANGE) {
        // Return the scaled amplitude
        return SCALE * (RANGE - distance);
    } else {
        // Return 0 if the obstacle is out of range
        return 0.0f;
    }
}
