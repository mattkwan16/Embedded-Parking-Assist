#include "Sensor.h"
#include <iostream>
#include <thread>
#include <chrono>

Sensor::Sensor() : 
    running_(false),
    data_() {

}

Sensor::~Sensor() {
    stop();
}

void Sensor::start() {
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&Sensor::outputLoop, this);
    }
}

void Sensor::stop() {
    if (running_) {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }
}

Ping Sensor::ping() const {
    Ping p;
    // todo: keying
    // todo: amplitude shifting
    p.amplitude = 1.0f;
    p.key = "hash1";
    p.tof = 0.0f;
    return p;
}

void Sensor::updateData(Ping const &echo) {
    // Lock the mutex before updating the data
    std::lock_guard<std::mutex> lock(mtx_);
    data_ = echo;
}

void Sensor::outputLoop() {
    const int OUTPUT_PERIOD_MS = 200;
    while (running_) {
        auto start = std::chrono::high_resolution_clock::now();
        int duration = 0;
        while (true) {
            auto end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (duration >= OUTPUT_PERIOD_MS) {
                break;
            }
        }

        // Output closest obstacle
        // todo: ping goes to cpu
        std::cout << "Closest obstacle (amp, tof, key): ("
                  << data_.amplitude << ", " << data_.tof
                  << ", " << data_.key << ")" << std::endl;

        // Calculate the elapsed time
        auto end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Print the elapsed time to ensure it's approximately n milliseconds
        std::cout << "Elapsed time: " << duration << " milliseconds" << std::endl;
    }
}
