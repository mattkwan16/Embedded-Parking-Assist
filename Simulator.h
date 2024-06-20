#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Obstacle.h"
#include "Sensor.h"
#include "Cpu.h"
#include "Jammer.h"
#include <vector>
#include <mutex>

class Simulator {
public:
    Simulator();
    ~Simulator();
    void start();
    void stop();
    void jam();
    void addObstacle(const Obstacle& obstacle);
    void addSensor(Sensor* sensor);
    void addCpu(Cpu* cpu);
    void addJammer(Jammer* j);
    void pingObstacles() const;
    void clear();

private:
    std::vector<Obstacle> obstacles_;
    std::vector<Sensor*> sensors_;
    std::thread thread_;
    bool running_;
    Cpu* cpu_;
    Jammer* jammer_;
    mutable std::mutex mtx_;  // Protects obstacles_ for thread-safe access
};

#endif // SIMULATOR_H
