#include <iostream>
#include <time.h>
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
    simulator.clear();
}

// One out of range, one in range, then one closer in range
void test_case1(Simulator& simulator) {
    int NUM_TEST_SENSORS = 1;
    std::vector<Sensor> sensors(NUM_TEST_SENSORS);
    for (int i = 0; i < NUM_TEST_SENSORS; i++) {
        simulator.addSensor(&sensors[i]);
    }
    simulator.addObstacle(Obstacle(20.0f, 15.0f));
    simulator.addObstacle(Obstacle(1.5f, 3.0f));
    start_test(sensors, simulator);

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));

    simulator.addObstacle(Obstacle(1.5f, 1.0f));

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stop_test(sensors, simulator);
}

// 3 not in range, then 2 in range 
// TODO: allow changing/removing of the obstacles
void test_case2(Simulator& simulator) {
    int NUM_TEST_SENSORS = 3;
    std::vector<Sensor> sensors(NUM_TEST_SENSORS);
    for (int i = 0; i < NUM_TEST_SENSORS; i++) {
        simulator.addSensor(&sensors[i]);
    }
    simulator.addObstacle(Obstacle(10.0f, 5.0f));
    simulator.addObstacle(Obstacle(100.0f, 5.0f));
    simulator.addObstacle(Obstacle(15.0f, 1.0f));

    start_test(sensors, simulator);
    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));

    simulator.addObstacle(Obstacle(1.5f, 1.0f));
    simulator.addObstacle(Obstacle(0.5f, 1.0f));

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stop_test(sensors, simulator);
}

// TODO: add jammer (does not key)
// TODO: add jammer (echos key but doesnt adjust amplitude)

int main() {
    // Globally initialize rand()
    srand (time(NULL));

    Simulator simulator;
    Cpu cpu;
    simulator.addCpu(&cpu);

    std::cout << "\n\n Test Case 1: \n\n";
    test_case1(simulator);

    std::cout << "\n\n Test Case 2: \n\n";
    test_case2(simulator);

    return 0;
}
