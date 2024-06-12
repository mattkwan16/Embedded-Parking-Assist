#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Obstacle.h"
#include "Sensor.h"
#include <vector>
#include <mutex>

class Simulator {
public:
    Simulator();
    ~Simulator();
    void start();
    void stop();
    void addObstacle(const Obstacle& obstacle);
    void addSensor(Sensor* sensor);
    void pingObstacles() const;

private:
    std::vector<Obstacle> obstacles_;
    std::vector<Sensor*> sensors_;
    std::thread thread_;
    bool running_;
    mutable std::mutex mtx_;  // Protects obstacles_ for thread-safe access
};

#endif // SIMULATOR_H
