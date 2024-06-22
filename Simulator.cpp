#include "Simulator.h"
#include "Ping.h"
#include <iostream>
#include <limits>

Simulator::Simulator() : 
    running_(false),
    cpu_(nullptr),
    jammer_(nullptr) {
}

Simulator::~Simulator() {
    stop();
}

void Simulator::start() {
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&Simulator::pingObstacles, this);
    }
    if (cpu_) {
        cpu_->sensors(sensors_);
        cpu_->start();
    }
}

void Simulator::stop() {
    if (running_) {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    if (cpu_) {
        cpu_->stop();
    }
    if (jammer_) {
        jammer_->stop();
    }
}

void Simulator::jam() {
    if (jammer_) {
        jammer_->sensors(sensors_);
        jammer_->start();
    } else {
        std::cerr << "Error: No jammer to use!\n";
    }
}

void Simulator::clear() {
    sensors_.clear();
    obstacles_.clear();
    if (cpu_) {
        cpu_->clear();
    }
}

void Simulator::addObstacle(const Obstacle& obstacle) {
    std::lock_guard<std::mutex> lock(mtx_);
    obstacles_.push_back(obstacle);
}

void Simulator::addSensor(Sensor* s) {
    std::lock_guard<std::mutex> lock(mtx_);
    sensors_.push_back(s);
}

void Simulator::addCpu(Cpu* cpu) {
    cpu_ = cpu;
}

void Simulator::addJammer(Jammer* j) {
    jammer_ = j;
}

// Takes a true ping
// Scales amplitude based on obstacle 
// Provides time-of-flight (todo: have the sensors wait)
// Echos the key
// Simulation should watch sensor constantly if it flags for another ping (each beginning of output loop). 

void Simulator::pingObstacles() const {
    while (running_) {
        Ping echo;

        for (int i = 0; i < sensors_.size(); i++) {
            if (!sensors_[i]->ping_ready()) {
                continue;
            }
            // Ping obstacles from sensor
            Ping input = sensors_[i]->ping();

            for (const auto& obstacle : obstacles_) {
                echo = obstacle.ping(input);
                // Provide ping to jammer
                if (jammer_) {
                    jammer_->receivePing(input);
                }
                /*
                std::clog << "Simulator: Obstacle at (" << obstacle.x << ", " << obstacle.y 
                        << ") has amplitude: " << echo.amplitude << std::endl;
                */
                sensors_[i]->processEcho(echo);
            }
        }
    }
}
