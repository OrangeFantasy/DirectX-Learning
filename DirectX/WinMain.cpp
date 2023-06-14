#include "Window.h"

#include "resource.h"

int WINAPI WinMain(                   // WinMain
    _In_ HINSTANCE     hInstance,     // handle to current instance
    _In_opt_ HINSTANCE hPrevInstance, // handle to previous instance
    _In_ LPSTR         lpCmdLine,     // command line
    _In_ int           nCmdShow       // show state
)
{
    try
    {
        Window window(640, 480, TEXT("MainWindow"));

        // Message pump
        MSG  msg;
        BOOL gResult;
        while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (window.keyboard.KeyIsPressed(VK_MENU))
            {
                MessageBox(nullptr, TEXT("Something happen"), TEXT("key! "), MB_OK);
            }
            while (!window.mouse.IsEmpty())
            {
                Mouse::Event e = window.mouse.Read();
                switch (e.GetType())
                {
                case Mouse::Event::Type::LeaveWindow:
                    window.SetTitle(L"Leave.");
                    break;
                case Mouse::Event::Type::EnterWindow:
                    window.SetTitle(L"Enter.");
                    break;
                case Mouse::Event::Type::WheelUp:
                    window.SetTitle(L"Up");
                    break;
                case Mouse::Event::Type::Move:
                {
                    auto title = L"[ " + std::to_wstring(e.GetPosX()) + L", " + std::to_wstring(e.GetPosY()) + L" ]";
                    window.SetTitle(title);
                    break;
                }
                default:
                    break;
                }
            }
        }
        return (int)msg.wParam;
    }
    catch (const OraeException& ex)
    {
        MessageBox(nullptr, ex.What(), ex.GetType(), MB_OK);
        return -1;
    }
    catch (const std::exception& ex)
    {
        MessageBoxA(nullptr, ex.what(), nullptr, MB_OK);
        return -1;
    }
}
