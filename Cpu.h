#ifndef CPU_H
#define CPU_H

#include <vector>
#include "TaskControlBlock.h"
#include "Sensor.h"
#include <functional>

struct CpuContext {
    int process_iter;
    int display_iter;
    std::string oss;
    CpuContext () :
        process_iter(0),
        display_iter(1),
        oss("") {}
};

class Cpu {
public:
    Cpu();
    ~Cpu();
    void start();
    void stop();
    void display(CpuContext& c);
    bool process_ready(){ return process_ready_; };
    void addSensor(Sensor* s) { sensors_.push_back(s); }
    void clear() { sensors_.clear(); }
    void processSensors(CpuContext& c);
    enum Distance
    {
        EXTREMELY_CLOSE=1,
        VERY_CLOSE,
        SOMEWHAT_CLOSE,
        NOT_CLOSE,
        OUT_OF_RANGE
    };
private:
    static constexpr int TIME_SLICE_MS = 100;
    void processLoop();
    std::thread thread_;
    bool running_;
    bool process_ready_;
    std::vector<Distance> distances_;
    void prepDisplay();
    std::string s_;
    std::mutex mtx_;
    std::vector<Sensor*> sensors_;
    std::vector<TaskControlBlock<CpuContext>> tasks_;
    void addTask(std::function<void(CpuContext&)> task, int priority, std::chrono::milliseconds timeSlice);
    std::mutex task_mtx_;
};
#endif