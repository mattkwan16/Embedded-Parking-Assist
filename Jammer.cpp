#include "Jammer.h"
#include <iostream>
#include <thread>
#include <chrono>

Jammer::Jammer() : 
    running_(false){
}

Jammer::~Jammer() {
    stop();
}

void Jammer::start() {
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&Jammer::jam, this);
    }
}

void Jammer::stop() {
    if (running_) {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }
}

// Prepares a ping to be emitted to a sensor
Ping Jammer::ping(float amp, std::string key, float tof) {
    Ping p;
    p.amplitude = amp;
    p.key = key;
    p.tof = tof;

    // Display
    std::cout << "Jammer: Sent ping (amp, tof, key): ("
                << p.amplitude << ", " << p.tof
                << ", " << p.key << ")" << std::endl;

    return p;
}

Ping Jammer::ping(Ping p) {
    return ping(DEFAULT_AMP, p.key, DEFAULT_TOF);
}

// Saves a copy of the sensor ping's waveform to be echoed back
void Jammer::receivePing(Ping p) {
    copy_ = p;
}

// Pings a sensor directly using a copy of the sensor's ping
void Jammer::jam() {
    while (running_) {
        auto start = std::chrono::high_resolution_clock::now();
        int duration = 0;
        Ping echo;

        for (int i = 0; i < sensors_.size(); i++) {
            sensors_[i]->processEcho(this->ping(copy_));
        }
        while (true) {
            auto end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (duration >= Jammer::OUTPUT_PERIOD_MS) {
                break;
            }
        }

        // Calculate the elapsed time
        auto end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Print the elapsed time to ensure it's approximately n milliseconds
        std::cout << "Jammer elapsed time: " << duration << " milliseconds" << std::endl;
    }
}
