#include "App.h"

App::App() : Window(800, 600, TEXT("The Donkey Fart Box")) {}

int32_t App::Run()
{
    while (true)
    {
        if (std::optional<int32_t> errCode = FWindow::ProcessMessages())
        {
            // Quit or Error.
            return *errCode;
        }
        Tick();
    }
}

void App::Tick(float Delta)
{
    float c = std::sin(Timer.Peek()) / 2.0f + 0.5f;
    Window.Graphics().ClearBuffer(c, c, 1.0f);

    Window.Graphics().EndFrame();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
