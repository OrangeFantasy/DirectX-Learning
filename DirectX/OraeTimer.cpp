#include "OraeTimer.h"

using namespace std::chrono;

OraeTimer::OraeTimer()
{
    TimePoint = steady_clock::now();
}

float OraeTimer::Mark()
{
    const steady_clock::time_point OldTimePoint = TimePoint;
    TimePoint = steady_clock::now();
    const duration<float> frameTime = TimePoint - OldTimePoint;
    return frameTime.count();
}

float OraeTimer::Peek() const
{
    return duration<float>(steady_clock::now() - TimePoint).count();
}
