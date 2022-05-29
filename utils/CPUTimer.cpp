//
// Created by Abel Lee on 2022/5/29.
//

#include "CPUTimer.h"


CPUTimer::CPUTimer() = default;


CPUTimer::CPUTimer(const std::string &taskName) {
    this->taskName = taskName;
}



void CPUTimer::start() {
    if (!active) {
        active = true;
        count++;
        startTime = std::chrono::high_resolution_clock::now();
    }
}

void CPUTimer::stop() {
    if (active) {
        auto endTime = std::chrono::high_resolution_clock::now();
        lastTime = endTime - startTime;
        totalTime += lastTime;
        active = false;
    }
}

size_t CPUTimer::elapsedMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(totalTime).count();
}

size_t CPUTimer::elapsedSeconds() const {
    return std::chrono::duration_cast<std::chrono::seconds>(totalTime).count();
}

size_t CPUTimer::getCount() const {
    return count;
}

double CPUTimer::getAvgTime() const {
    size_t milliseconds = elapsedMilliseconds();
    return (double)milliseconds / getCount();
}

double CPUTimer::getFPS() const {
    return 1000.0 / getAvgTime();
}

void CPUTimer::print() const {
    if (!taskName.empty()) {
        std::cout << "Task Name = " << taskName << std::endl;
    }
    std::cout << "Avg time cost = " << std::to_string(getAvgTime()) << "ms" << std::endl;
    std::cout << "FPS = " << std::to_string(getFPS()) << std::endl;
}