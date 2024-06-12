#include "Simulator.h"
#include "Ping.h"
#include <iostream>
#include <limits>

Simulator::Simulator() {}

Simulator::~Simulator() {}

void Simulator::addObstacle(const Obstacle& obstacle) {
    std::lock_guard<std::mutex> lock(mtx_);
    obstacles_.push_back(obstacle);
}

void Simulator::addSensor(Sensor* s) {
    std::lock_guard<std::mutex> lock(mtx_);
    sensors_.push_back(s);
}

// Takes a true ping
// Scales amplitude based on obstacle 
// Provides time-of-flight (todo: have the sensors wait)
// Echos the key
void Simulator::pingObstacles() const {
    std::lock_guard<std::mutex> lock(mtx_);
    Ping echo;
    Ping closest;
    closest.tof = std::numeric_limits<float>::max();
    for (int i = 0; i < sensors_.size(); i++) {
        Ping input = sensors_[i]->ping();
        for (const auto& obstacle : obstacles_) {
            echo = obstacle.ping(input);
            std::cout << "Simulator: Obstacle at (" << obstacle.x << ", " << obstacle.y 
                    << ") has amplitude: " << echo.amplitude << std::endl;
            // todo: move this intelligence to sensor
            if (echo.tof != 0.0f && echo.amplitude != 0.0f &&
                echo.tof < closest.tof) {
                closest = echo;
            }
        }
        sensors_[i]->updateData(closest);
    }
}
