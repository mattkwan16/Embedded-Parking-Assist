#ifndef SENSOR_H
#define SENSOR_H

#include <thread>

class Sensor {
public:
    Sensor();
    ~Sensor();

    void start();
    void stop();
    void updateData(float data);

private:
    void outputLoop();

    std::thread thread_;
    bool running_;

    float data_;
    std::mutex mtx_;
};

#endif // SENSOR_H
