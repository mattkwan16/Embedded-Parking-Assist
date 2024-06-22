#include "Cpu.h"
#include "Sensor.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

Cpu::Cpu() :
    running_(false) {
    // Process Sensors every time slice
    addTask(std::bind(&Cpu::processSensors, this, std::placeholders::_1), 
        1, std::chrono::milliseconds(Cpu::TIME_SLICE_MS));
    // Prep display every time slice
    addTask(std::bind(&Cpu::prepDisplay, this, std::placeholders::_1), 
        1, std::chrono::milliseconds(Cpu::TIME_SLICE_MS));
    // Display (throttled)
    addTask(std::bind(&Cpu::display, this, std::placeholders::_1), 
        1, std::chrono::milliseconds(Cpu::TIME_SLICE_MS));
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
}

// Eg. cpu must be able to take in at least 8 sensors and process them 
// concurrently and output twice per second. 
// Input: Proximity information relative to 8 sensors as a vector of 8 float32 values
// Output: 1-5 scale of distance from each sensor displayed on console
void Cpu::display(CpuContext& c) {
    //TODO: display the positions of the obstacles?
    // todo: add context of how far along it is
    if (std::chrono::steady_clock::now() - c.last_display < 
        std::chrono::milliseconds(Cpu::DISPLAY_FREQ_MS)) {
        return;
    }
    // Otherwise, it is ready to display
    std::lock_guard<std::mutex> lg(this->mtx_);
    std::cout << s_ << std::endl;
    c.last_display = std::chrono::steady_clock::now();
    s_ = "\n\nNo new information available.\n\n";
}

void Cpu::processSensors(CpuContext& context) {
    auto start = std::chrono::steady_clock::now();

    const float EXTREMELY_CLOSE_M = 1.0f;
    const float VERY_CLOSE_M = 2.0f;
    const float SOMEWHAT_CLOSE_M = 3.0f;
    const float NOT_CLOSE_M = 4.0f;

    //distances_.clear();
    distances_.resize(sensors_.size()); // todo, better init
    for (int i = context.process_iter; i < sensors_.size(); i++) {
        auto& sensor = sensors_[i];
        // Check for "interrupt"
        if (std::chrono::steady_clock::now() - start >= std::chrono::milliseconds(Cpu::TIME_SLICE_MS)) {
            // Save context
            context.process_iter = i;
            return;
        }
        Ping p = sensor->data();
        float distance_m = p.tof * SPEED_OF_SOUND_MPS;
        if (distance_m > NOT_CLOSE_M || distance_m == 0.0f) {
            distances_[i] = OUT_OF_RANGE;
        } else if (distance_m > SOMEWHAT_CLOSE_M) {
            distances_[i] = NOT_CLOSE;
        } else if (distance_m > VERY_CLOSE_M) {
            distances_[i] = SOMEWHAT_CLOSE;
        } else if (distance_m > EXTREMELY_CLOSE_M) {
            distances_[i] = VERY_CLOSE;
        } else {
            distances_[i] = EXTREMELY_CLOSE;
        }
        // Add artifical processing delay to simulate DSP
        std::this_thread::sleep_for(std::chrono::milliseconds(int(0.99*Cpu::TIME_SLICE_MS)));
    }

    // reset context if it makes it to end
    context.process_iter = 0;
    return;
}

// Prepping the display means the cpu can keep up
void Cpu::prepDisplay(CpuContext& context) {
    auto start = std::chrono::steady_clock::now();
    // initialize with context
    std::stringstream oss(context.oss);
    for (int i = context.display_iter; i <= distances_.size(); i++) {
        if (std::chrono::steady_clock::now() - start >= std::chrono::milliseconds(TIME_SLICE_MS)) {
            // Save to context and quit if out of time
            context.oss = oss.str().c_str();
            context.display_iter = i;
            return;
        }
        std::string stars(distances_[i-1], '*');
        if (distances_[i-1] == OUT_OF_RANGE) {
            stars = "--- n/a ---";
        }
        oss << "\nSensor " << i << " distance: "
                  << "| " << stars << " |\n\n";
    }
    std::lock_guard<std::mutex> lg(this->mtx_);
    s_ = oss.str().c_str();
    // Reset context if finish
    context.oss = "";
    context.display_iter = 1;
    return;
}

void Cpu::addTask(std::function<void(CpuContext&)> task, int priority, std::chrono::milliseconds timeSlice) {
    tasks_.push_back({task, READY, priority, CpuContext(), timeSlice, std::chrono::steady_clock::now()});
}

void Cpu::processLoop() {
    int i_task = 0;
    while (running_) {
        int duration = 0;
        // Waiting-for-sensors window
        auto& task = tasks_[i_task];
        auto start = std::chrono::steady_clock::now();
        task.task(task.context);
        // Wait for time slice window to end
        while (std::chrono::steady_clock::now() - start < task.timeSlice) {
            // Future work: Do background work here
        }

        // Calculate the elapsed time
        auto end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Print the elapsed time to ensure it's approximately n milliseconds
        std::cout << "CPU task " << i_task << " elapsed time: " << duration << " milliseconds" << std::endl;
        if (i_task >= tasks_.size() - 1) {\
            i_task = 0;
        } else {
            i_task++;
        }
    }
}