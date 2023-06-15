#pragma once

#include "Window.h"
#include "OraeTimer.h"

class App
{
public:
    App();

    int32_t Run();

private:
    void Tick(float Delta = 20.0f);

private:
    FWindow Window;
    OraeTimer Timer;
};
