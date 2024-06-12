#ifndef SENSOR_H
#define SENSOR_H

#include <mutex>
#include <thread>
#include "Ping.h"

/*  Sensor.h

    Sensors add noise but provides a moving average every ___ms
    Sensors detect false echos and keys the pings
    Simulation calls Sensor's ping() function which returns the closest obstacle

*/
class Sensor {
public:
    Sensor();
    ~Sensor();

    void start();
    void stop();
    void updateData(float data);
    Ping ping() const;

private:
    void outputLoop();

    std::thread thread_;
    bool running_;

    float data_;
    std::mutex mtx_;
};

#endif // SENSOR_H
