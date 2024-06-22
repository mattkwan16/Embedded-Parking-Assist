#include "Sensor.h"
#include <iostream>
#include <thread>
#include <chrono>

Sensor::Sensor() : 
    running_(false),
    ping_ready_(false),
    data_(Ping()) {

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
    ping_ready_ = false;
}

float Sensor::amplitudeShift() {
    const float MIN_AMP = 1.5f;
    // max must be int for mod
    const int MAX_AMP = 5;
    return MIN_AMP + float(rand() % MAX_AMP);
}

std::string Sensor::key() {
    const int LEN_POSSIBLE_CHARS = 62;
    const int LEN_HASH = 7;
    std::string s(LEN_HASH, 0); 
    std::generate(s.begin(), s.end(), []{
        return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"[rand() % LEN_POSSIBLE_CHARS];
        }); 
    return s;
}

Ping Sensor::ping() {
    Ping p;
    p.amplitude = amplitudeShift();
    p.key = key();
    p.tof = 0.0f;

    // Display
    std::cout << "Sent ping (amp, tof, key): ("
                << p.amplitude << ", " << p.tof
                << ", " << p.key << ")" << std::endl;

    // Reset previous ping information
    ping_ready_ = false;
    sentData_ = p;
    // Note that data_ will hold onto the latest one
    // todo (optional): remove data_ when it becomes stale

    return p;
}

// processEcho: update data iff it has correctly scaled amplitude,
// matches the hash, and is closer than the other pings
void Sensor::processEcho(Ping const &echo) {
    if (!running_) { 
        return;
    }
    // Empty echo check
    if (echo.tof == 0.0f || echo.tof == std::numeric_limits<float>::max() ||
        echo.amplitude <= 0.0f) {
        return;
    }

    // Cybersecurity check
    if (echo.amplitude >= sentData_.amplitude ||
        echo.key != sentData_.key) {
        std::cout << "Sensor ignored suspicious echo with key: " << echo.key << std::endl;
        return;
    }

    // Data is valid, but is it closer than existing obstacle?
    if (echo.tof > data_.tof) {
        std::cout << "Sensor: " << data_.key << " is closer than " << echo.key << std::endl;
        return;
    }
    updateData(echo);
}

void Sensor::updateData(Ping const &echo) {
    // Lock the mutex before updating the data
    std::lock_guard<std::mutex> lock(mtx_);
    data_ = echo;
}

bool Sensor::ping_ready() const {
    return ping_ready_;
}

// Output loop of n ms is the listening window (set flag then wait). 
// Then it outputs to console and cpu then flag again.
void Sensor::outputLoop() {
    while (running_) {
        // Set ping flag for sim
        ping_ready_ = true;
        auto start = std::chrono::high_resolution_clock::now();
        int duration = 0;
        // Listening window
        while (true) {
            auto end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (duration >= Sensor::OUTPUT_PERIOD_MS) {
                break;
            }
        }
        // Output closest obstacle
        std::cout << "Closest obstacle (amp, tof, key): ("
                  << data_.amplitude << ", " << data_.tof
                  << ", " << data_.key << ")" << std::endl;

        // Calculate the elapsed time
        auto end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Print the elapsed time to ensure it's approximately n milliseconds
        std::cout << "Sensor elapsed time: " << duration << " milliseconds" << std::endl;
    }
}
