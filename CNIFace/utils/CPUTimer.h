//
// Created by Abel Lee on 2022/5/29.
//

#ifndef CNIFACE_CPUTIMER_H
#define CNIFACE_CPUTIMER_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

class CPUTimer {
public:
    CPUTimer();

    explicit CPUTimer(const std::string& taskName);

    void start();

    void stop();

    size_t elapsedMilliseconds() const;

    size_t elapsedSeconds() const;

    size_t getCount() const ;

    double getAvgTime() const;

    double getFPS() const ;

    void print() const ;

private:
    std::string taskName = "";
    std::chrono::nanoseconds totalTime = std::chrono::steady_clock::duration::zero();
    std::chrono::nanoseconds lastTime = std::chrono::steady_clock::duration::zero();
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    size_t count = 0;
    bool active = false;
};


#endif //CNIFACE_CPUTIMER_H
