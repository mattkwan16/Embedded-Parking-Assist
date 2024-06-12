#include "Simulator.h"
#include <iostream>

Simulator::Simulator() {}

Simulator::~Simulator() {}

void Simulator::addObstacle(const Obstacle& obstacle) {
    std::lock_guard<std::mutex> lock(mtx_);
    obstacles_.push_back(obstacle);
}

void Simulator::pingObstacles() const {
    std::lock_guard<std::mutex> lock(mtx_);
    for (const auto& obstacle : obstacles_) {
        float amplitude = obstacle.ping();
        std::cout << "Obstacle at (" << obstacle.x << ", " << obstacle.y 
                  << ") has amplitude: " << amplitude << std::endl;
    }
}

