#include "NONG/time.h"
#include <SDL3/SDL.h>

namespace NONG{
    void TimeController::UpdateTime()
    {
        Time::Update();
    }
    
    void Time::Update()
    {
        _lastFrameTimeNs = _currentFrameTimeNs;
        _currentFrameTime = SDL_GetTicksNS();
        _deltaTimeNs = _currentFrameTimeNs - _lastFrameTimeNs;

        _deltaTime = _deltaTimeNs / 1000000000.0;
        _currentFrameTime = _currentFrameTime / 1000000000.0;
    }

    double Time::deltaTime() { return _deltaTime; }
    uint64_t Time::deltaTimeNs() { return _deltaTimeNs; }
    double Time::time() { return _currentFrameTime; }
    uint64_t Time::timeNs() { return _currentFrameTimeNs; }
}