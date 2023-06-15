#pragma once

#include <chrono>

class OraeTimer
{
public:
    OraeTimer();

    float Mark();
    float Peek() const;

private:
    std::chrono::steady_clock::time_point TimePoint;
};
