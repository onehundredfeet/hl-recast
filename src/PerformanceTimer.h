#ifndef __PERFORMANCE_TIMER_H_
#define __PERFORMANCE_TIMER_H_
#include "PerfTimer.h"
class PerformanceTimer {
    TimeVal _start;
    TimeVal _stop;

   public:
    PerformanceTimer() {}
    ~PerformanceTimer() {}
    void start() {
        _start = getPerfTime();
    }
    void stop() {
        _stop = getPerfTime();
    }
    double deltaSeconds() {
        int microSeconds = getPerfTimeUsec(_stop - _start);

        return (double)microSeconds / 1000000;
    }

    double deltaMilliseconds() {
        int microSeconds = getPerfTimeUsec(_stop - _start);

        return (double)microSeconds / 1000;
    }

    double deltaMicroseconds() {
        int microSeconds = getPerfTimeUsec(_stop - _start);

        return (double)microSeconds;
    }
};
#endif