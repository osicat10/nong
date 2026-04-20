#ifndef NONG_TIME_H
#define NONG_TIME_H

#include <cstdint>

namespace NONG {
    class TimeController
    {
    private:
        TimeController();
    public:
        static void UpdateTime();
    };

    class Time
    {
    private:
        Time();
        static uint64_t _lastFrameTimeNs;
        static uint64_t _currentFrameTimeNs;
        static uint64_t _deltaTimeNs;
        static double _deltaTime;
        static double _currentFrameTime;
        
        static void Update();
    public:
        static double deltaTime();
        static uint64_t deltaTimeNs();
        static double time();
        static uint64_t timeNs();

        friend class TimeController;
    };
}

#endif