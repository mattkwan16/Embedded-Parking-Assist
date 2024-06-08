#include <iostream>
#include "Sensor.h"
#include "Obstacle.h"
#include "Simulator.h"

int main() {
    Sensor sensor;
    sensor.start();

    Simulator simulator;
    simulator.addObstacle(Obstacle(10.0f, 5.0f, 5.0f));
    simulator.addObstacle(Obstacle(20.0f, 15.0f, 3.0f));

    // Simulate some sensor updates
    sensor.updateData(1.11);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    sensor.updateData(2.22);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Ping obstacles
    simulator.pingObstacles();

    sensor.stop();
    return 0;
}
