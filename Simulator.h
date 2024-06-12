#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Obstacle.h"
#include <vector>
#include <mutex>

class Simulator {
public:
    Simulator();
    ~Simulator();

    void addObstacle(const Obstacle& obstacle);
    void pingObstacles() const;

private:
    std::vector<Obstacle> obstacles_;
    mutable std::mutex mtx_;  // Protects obstacles_ for thread-safe access
};

#endif // SIMULATOR_H
