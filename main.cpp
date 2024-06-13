#include <iostream>
#include "Sensor.h"
#include "Obstacle.h"
#include "Cpu.h"
#include "Simulator.h"

void start_test(std::vector<Sensor>& sensors, Simulator& simulator) {
    simulator.start();
    for (int i = 0; i < sensors.size(); i++) {
        sensors[i].start();
    }
}

void stop_test(std::vector<Sensor>& sensors, Simulator& simulator) {
    for (int i = 0; i < sensors.size(); i++) {
        sensors[i].stop();
    }
    simulator.stop();
}

// One out of range, one in range, then one closer in range
void test_case1(Simulator& simulator) {
    int NUM_TEST_SENSORS = 1;
    std::vector<Sensor> sensors(NUM_TEST_SENSORS);

    start_test(sensors, simulator);
    simulator.addObstacle(Obstacle(20.0f, 15.0f));
    simulator.addObstacle(Obstacle(1.0f, 5.0f));
    simulator.addSensor(&sensors[0]);

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));

    simulator.addObstacle(Obstacle(1.5f, 1.0f));

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stop_test(sensors, simulator);
}

int main() {
    Simulator simulator;
    Cpu cpu;
    simulator.addCpu(&cpu);

    test_case1(simulator);

    return 0;
}
