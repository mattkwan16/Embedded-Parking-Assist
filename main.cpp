#include <iostream>
#include <time.h>
#include "Sensor.h"
#include "Obstacle.h"
#include "Cpu.h"
#include "Simulator.h"
#include "Jammer.h"

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
    std::cout << "Test complete. Type y to continue.\n";
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
// One out of range, one in range, then one closer in range. Jammer.
void test_case3(Simulator& simulator) {
    Jammer jammer;
    simulator.addJammer(&jammer);
    int NUM_TEST_SENSORS = 1;
    std::vector<Sensor> sensors(NUM_TEST_SENSORS);
    for (int i = 0; i < NUM_TEST_SENSORS; i++) {
        simulator.addSensor(&sensors[i]);
    }
    simulator.addObstacle(Obstacle(20.0f, 15.0f));
    simulator.addObstacle(Obstacle(1.5f, 3.0f));
    start_test(sensors, simulator);
    simulator.jam();
    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));

    simulator.addObstacle(Obstacle(1.5f, 1.0f));

    // Let the threads run
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stop_test(sensors, simulator);
}

int main() {
    // Globally initialize rand()
    srand (time(NULL));

    Simulator simulator;
    Cpu cpu;
    simulator.addCpu(&cpu);

    std::string in;
    std::cout << "Type y to continue.\n";
    std::cout << "\n\n Test Case 1: One out of range, one in range, then one closer in range\n\n";
    std::cin >> in;
    test_case1(simulator);

    std::cout << "\n\n Test Case 2: 3 not in range, then 2 in range \n\n";
    std::cin >> in;
    test_case2(simulator);
    std::cin >> in;

    std::cout << "\n\n Test Case 3: One out of range, one in range, then one closer in range. Jammer.\n\n";
    test_case3(simulator);

    return 0;
}
