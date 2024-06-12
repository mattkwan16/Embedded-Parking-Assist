#include "Simulator.h"
#include "Ping.h"
#include <iostream>

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
    Ping p;
    float closest_obstacle_amp = 0.0f;
    for (int i = 0; i < sensors_.size(); i++) {
        Ping input = sensors_[i]->ping();
        for (const auto& obstacle : obstacles_) {
            p = obstacle.ping(input);
            std::cout << "Simulator: Obstacle at (" << obstacle.x << ", " << obstacle.y 
                    << ") has amplitude: " << p.amplitude << std::endl;
            if (p.amplitude != 0.0f && p.amplitude < closest_obstacle_amp) {
                closest_obstacle_amp = p.amplitude;
            }
        }
    }
}

