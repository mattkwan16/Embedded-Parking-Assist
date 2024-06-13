#include "Obstacle.h"
#include "Ping.h"
#include <iostream>
#include <cmath>

#define RANGE 10.0f
#define SCALE 1.0f 

Obstacle::Obstacle(float x, float y) : x(x), y(y) {}

// Echo the ping, but
// scale the amplitude based on distance
Ping Obstacle::ping(const Ping &input) const {
    Ping output = input;
    // Calculate the distance from the origin (0, 0)
    float distance = std::sqrt(x * x + y * y);

    if (distance <= RANGE) {
        // Return the scaled amplitude
        output.amplitude = SCALE * (RANGE - distance)/RANGE * input.amplitude;
        output.tof = distance/SPEED_OF_SOUND_MPS;
        return output;
    } else {
        // Return 0 if the obstacle is out of range
        output.amplitude = 0.0f;
        output.tof = 0.0f;
        return output;
    }
}
