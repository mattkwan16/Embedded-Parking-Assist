#include "Cpu.h"
#include "Sensor.h"
#include <iostream>
#include <thread>
#include <chrono>

Cpu::Cpu() :
    running_(false) {

}

Cpu::~Cpu() {
    stop();
}


void Cpu::start() {
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&Cpu::processLoop, this);
    }
}

void Cpu::stop() {
    if (running_) {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    process_ready_ = false;
}

// Eg. cpu must be able to take in at least 8 sensors and process them 
// concurrently and output twice per second. 
// Input: Proximity information relative to 8 sensors as a vector of 8 float32 values
// Output: 1-5 scale of distance from each sensor displayed on console
void Cpu::display() {
    //TODO: display the positions of the obstacles?
    for (int i = 1; i <= distances_.size(); i++) {
        std::string ruler = "||";
        ruler.insert(1, distances_[i-1], '*');
        if (distances_[i-1] == OUT_OF_RANGE) {
            ruler = "|--- n/a ---|";
        }
        std::cout << "\nSensor " << i << " distance: "
                  << ruler << "\n\n";
    }
}

void Cpu::processSensors(std::vector<Sensor*> sensors) {
    const float EXTREMELY_CLOSE_M = 1.0f;
    const float VERY_CLOSE_M = 2.0f;
    const float SOMEWHAT_CLOSE_M = 3.0f;
    const float NOT_CLOSE_M = 4.0f;

    distances_.clear();
    for (const auto& sensor : sensors) {
        Ping p = sensor->data();
        float distance_m = p.tof * SPEED_OF_SOUND_MPS;
        if (distance_m > NOT_CLOSE_M || distance_m == 0.0f) {
            distances_.push_back(OUT_OF_RANGE);
        } else if (distance_m > SOMEWHAT_CLOSE_M) {
            distances_.push_back(NOT_CLOSE);
        } else if (distance_m > VERY_CLOSE_M) {
            distances_.push_back(SOMEWHAT_CLOSE);
        } else if (distance_m > EXTREMELY_CLOSE_M) {
            distances_.push_back(VERY_CLOSE);
        } else {
            distances_.push_back(EXTREMELY_CLOSE);
        }
    }
}

void Cpu::processLoop() {
    const int OUTPUT_PERIOD_MS = 500;
    while (running_) {
        // Set ping flag for sim
        process_ready_ = true;
        auto start = std::chrono::high_resolution_clock::now();
        int duration = 0;
        // Waiting-for-sensors window
        while (true && running_) {
            auto end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            if (duration >= OUTPUT_PERIOD_MS) {
                break;
            }
        }

        display();

        // Calculate the elapsed time
        auto end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Print the elapsed time to ensure it's approximately n milliseconds
        std::cout << "CPU elapsed time: " << duration << " milliseconds" << std::endl;
    }
}