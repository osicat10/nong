#include "NONG/time.h"
#include <SDL3/SDL.h>

namespace NONG{
    void TimeController::UpdateTime()
    {
        Time::Update();
    }

    uint64_t Time::_lastFrameTimeNs = 0;
    uint64_t Time::_currentFrameTimeNs = 0;
    uint64_t Time::_deltaTimeNs = 0;
    double Time::_deltaTime = 0;
    double Time::_currentFrameTime = 0;
    
    void Time::Update()
    {
        _lastFrameTimeNs = _currentFrameTimeNs;
        _currentFrameTimeNs = SDL_GetTicksNS();
        _deltaTimeNs = _currentFrameTimeNs - _lastFrameTimeNs;

        _deltaTime = _deltaTimeNs / 1000000000.0;
        _currentFrameTime = _currentFrameTimeNs / 1000000000.0;
    }

    double Time::deltaTime() { return _deltaTime; }
    uint64_t Time::deltaTimeNs() { return _deltaTimeNs; }
    double Time::time() { return _currentFrameTime; }
    uint64_t Time::timeNs() { return _currentFrameTimeNs; }
}