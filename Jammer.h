#ifndef JAMMER_H
#define JAMMER_H

#include "Sensor.h"
#include "Ping.h"
#include <thread>
#include <string>

/*  Jammer.h
// TODO: add jammer (does not key)
// TODO: add jammer (echos key but doesnt adjust amplitude)

*/
class Jammer {
public:
    Jammer();
    ~Jammer();
    void start();
    void stop();
    void jam();
    void sensors(std::vector<Sensor*> s) { sensors_ = s; }
    static constexpr int OUTPUT_PERIOD_MS = 20;

private:
    Ping ping(float a = 9.0f, std::string s = "jjjjjj", float tof = 0.5f);
    bool running_;
    std::thread thread_;
    std::vector<Sensor*> sensors_;
};

#endif // JAMMER_H
