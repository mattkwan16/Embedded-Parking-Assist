#include <iostream>
#include "Sensor.h"
#include "Obstacle.h"
#include "Simulator.h"

int main() {
    Sensor sensor;

    Simulator simulator;
    simulator.addObstacle(Obstacle(20.0f, 15.0f));
    simulator.addObstacle(Obstacle(1.0f, 5.0f));
    simulator.addSensor(&sensor);
    simulator.start();
    sensor.start();

    // Ping obstacles
    // todo: ping obstacles every n ms

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));

    simulator.addObstacle(Obstacle(1.5f, 1.0f));

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));

    sensor.stop();
    simulator.stop();
    return 0;
}
