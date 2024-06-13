#ifndef CPU_H
#define CPU_H

#include "Sensor.h"

class Cpu {
public:
    Cpu();
    ~Cpu();
    void start();
    void stop();
    void display();
    bool process_ready(){ return process_ready_; };
    void processSensors(std::vector<Sensor*> s);
    enum Distance
    {
        EXTREMELY_CLOSE=1,
        VERY_CLOSE,
        SOMEWHAT_CLOSE,
        NOT_CLOSE,
        OUT_OF_RANGE
    };
private:
    void processLoop();
    std::thread thread_;
    bool running_;
    bool process_ready_;
    std::vector<Distance> distances_;
    void prepDisplay();
    std::string s_;
    std::mutex mtx_;
};
#endif