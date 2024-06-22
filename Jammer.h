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
    static constexpr float DEFAULT_AMP = 9.0f;
    static constexpr float DEFAULT_TOF = 0.5f;
    inline static const std::string DEFAULT_KEY = "jjjjjjj";
    void receivePing(Ping p);

private:
    Ping ping(float a = DEFAULT_AMP, std::string s = DEFAULT_KEY, float tof = DEFAULT_TOF);
    Ping ping(Ping p);
    bool running_;
    std::thread thread_;
    std::vector<Sensor*> sensors_;
    Ping copy_;
};

#endif // JAMMER_H
