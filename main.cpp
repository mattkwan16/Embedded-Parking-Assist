// Compile instructions in README

#include <iostream>
#include <thread>
#include <chrono>
#include "Sensor.h"

int main() {
    Sensor sensor;
    sensor.start();

    sensor.updateData(1.11);
    // Sleep for a while to let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sensor.updateData(2.22);
    // Sleep for a while to let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));

    sensor.stop();
    return 0;
}
