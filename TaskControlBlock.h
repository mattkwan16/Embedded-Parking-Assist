#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>

enum TaskState { READY, RUNNING, SLEEPING };

template<typename T> 
struct TaskControlBlock {
    std::function<void(T&)> task;
    TaskState state;
    // Future work: Add priority
    int priority;
    T context;
    std::chrono::milliseconds timeSlice;
    std::chrono::time_point<std::chrono::steady_clock> wakeUpTime;
};
