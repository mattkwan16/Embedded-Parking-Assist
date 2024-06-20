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
    void processEcho(Ping const& echo);
    void updateData(Ping const& data);
    Ping data() { return data_; };
    Ping ping();
    bool ping_ready() const;
    static constexpr int OUTPUT_PERIOD_MS = 80;

private:
    void outputLoop();
    float amplitudeShift();
    std::string key();

    std::thread thread_;
    bool running_;
    bool ping_ready_;

    Ping data_;
    Ping sentData_;
    std::mutex mtx_;
};

#endif // SENSOR_H
