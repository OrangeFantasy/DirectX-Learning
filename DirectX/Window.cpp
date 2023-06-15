#include "Window.h"

#include <sstream>

#include "resource.h"

// Error exception macro.
#define HWND_EXCEPT(Result) FWindow::FWindowException(__LINE__, TEXT(__FILE__), Result)
#define HWND_LAST_EXCEPT() FWindow::FWindowException(__LINE__, TEXT(__FILE__), GetLastError())

FWindow::FWindowClass FWindow::FWindowClass::WindowClass;

FWindow::FWindowClass::FWindowClass() noexcept : Instance(GetModuleHandle(nullptr))
{
    WNDCLASSEX WndClass    = {0};
    WndClass.cbSize        = sizeof(WndClass);
    WndClass.style         = CS_OWNDC;
    WndClass.lpfnWndProc   = HandleMsgSetup;
    WndClass.cbClsExtra    = 0;
    WndClass.cbWndExtra    = 0;
    WndClass.hInstance     = GetInstance();
    WndClass.hIcon         = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_ICON1));
    WndClass.hCursor       = nullptr;
    WndClass.hbrBackground = nullptr;
    WndClass.lpszMenuName  = nullptr;
    WndClass.lpszClassName = GetName();
    WndClass.hIconSm       = nullptr;

    RegisterClassEx(&WndClass);
}

FWindow::FWindowClass::~FWindowClass()
{
    UnregisterClass(WindowClassName, GetInstance());
}

const FChar* FWindow::FWindowClass::GetName() noexcept
{
    return WindowClassName;
}

HINSTANCE FWindow::FWindowClass::GetInstance() noexcept
{
    return WindowClass.Instance;
}

FWindow::FWindow(int32_t Width, int32_t Height, const FChar* WindowName) : Width(Width), Height(Height)
{
    // Calculate window size based on desired client region size
    RECT WindowRect   = {};
    WindowRect.left   = 100;
    WindowRect.right  = Width + WindowRect.left;
    WindowRect.top    = 100;
    WindowRect.bottom = Height + WindowRect.top;
    if (!AdjustWindowRectEx(&WindowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE, 0))
    {
        throw WIN32_LAST_EXCEPT();
    }

    // Create window & Get hWnd
    WindowHandle = CreateWindow(FWindowClass::GetName(), //
        WindowName,                                      //
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,        //
        CW_USEDEFAULT,                                   //
        CW_USEDEFAULT,                                   //
        WindowRect.right - WindowRect.left,              //
        WindowRect.bottom - WindowRect.top,              //
        nullptr,                                         //
        nullptr,                                         //
        FWindowClass::GetInstance(),                     //
        this                                             //
    );
    if (WindowHandle == nullptr)
    {
        throw WIN32_LAST_EXCEPT();
    }

    // Show window.
    ShowWindow(WindowHandle, SW_SHOW);

    // Create graphics object;
    GraphicsPtr = new FGraphics(WindowHandle);
}

FWindow::~FWindow()
{
    if (GraphicsPtr != nullptr)
    {
        delete GraphicsPtr;
        GraphicsPtr = nullptr;
    }

    DestroyWindow(WindowHandle);
}

std::optional<int32_t> FWindow::ProcessMessages()
{
    MSG Msg;

    // While queue has messages, remove and dispatch them (but do not block on empty queue).
    while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
    {
        // Check for quit because peekmessage does not signal this via return val.
        if (Msg.message == WM_QUIT)
        {
            return (int32_t)Msg.wParam;
        }

        // TranslateMessage will post auxilliary WM_CHAR messages from key msgs.
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    // return empty optional when not quitting app.
    return std::optional<int32_t>();
}

void FWindow::SetTitle(const FString Title)
{
    if (SetWindowText(WindowHandle, Title.c_str()) == 0)
    {
        throw WIN32_LAST_EXCEPT();
    }
}

FGraphics& FWindow::Graphics()
{
    if (GraphicsPtr == nullptr)
    {
        throw NO_GRAPHICS_EXCEPT();
    }
    return *GraphicsPtr;
}

LRESULT WINAPI FWindow::HandleMsgSetup(_In_ HWND HWnd, _In_ UINT Msg, _In_ WPARAM WParam, _In_ LPARAM LParam) noexcept
{
    // Use create parameter passed in from CreateWindow() to store window class pointer.
    if (Msg == WM_NCCREATE)
    {
        // Extract ptr to window class from creation data.
        const CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(LParam);
        FWindow*            pWnd    = static_cast<FWindow*>(pCreate->lpCreateParams);

        // Set WinAPI-managed user data to store ptr to window class.
        SetWindowLongPtr(HWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

        // Set message proc to normal (non-setup) handler now that setup is finished.
        SetWindowLongPtr(HWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&FWindow::HandleMsgThunk));

        // Forward message to window class handler.
        return pWnd->HandleMsg(HWnd, Msg, WParam, LParam);
    }
    // If we get a messgae before the WM_NCCREATE message, handle with default handler.
    return DefWindowProc(HWnd, Msg, WParam, LParam);
}

LRESULT WINAPI FWindow::HandleMsgThunk(_In_ HWND HWnd, _In_ UINT Msg, _In_ WPARAM WParam, _In_ LPARAM LParam) noexcept
{
    // Retrieve ptr to window class.
    FWindow* WindowPtr = reinterpret_cast<FWindow*>(GetWindowLongPtr(HWnd, GWLP_USERDATA));

    // Forward message to window class handler.
    return WindowPtr->HandleMsg(HWnd, Msg, WParam, LParam);
}

LRESULT FWindow::HandleMsg(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam) noexcept
{
    switch (Msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:
        Keyboard.ClearState();
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (!(LParam & 0x40000000) || Keyboard.AutorepeatIsEnable())
        {
            Keyboard.OnKeyPressed(static_cast<uint8_t>(WParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        Keyboard.OnKeyReleased(static_cast<uint8_t>(WParam));
        break;
    case WM_CHAR:
        Keyboard.OnChar(static_cast<FChar>(WParam));
        break;

    case WM_MOUSEMOVE:
    {
        POINTS Point = MAKEPOINTS(LParam);
        // In window.
        if (Point.x >= 0 && Point.x < Width && Point.y > 0 && Point.y < Height)
        {
            Mouse.OnMouseMove(Point.x, Point.y);
            if (!Mouse.IsInWindow())
            {
                SetCapture(HWnd);
                Mouse.OnMouseEnterWindow();
            }
        }
        // Not in window -> log move/maintain capture if button down.
        else
        {
            if (WParam & (MK_LBUTTON | MK_RBUTTON))
            {
                Mouse.OnMouseMove(Point.x, Point.y);
            }
            else
            {
                ReleaseCapture();
                Mouse.OnMouseLeaveWindow();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        Mouse.OnLeftPressed();
        break;
    }
    case WM_LBUTTONUP:
    {
        Mouse.OnLeftReleased();
        break;
    }
    case WM_RBUTTONDOWN:
    {
        Mouse.OnRightPressed();
        break;
    }
    case WM_RBUTTONUP:
    {
        Mouse.OnRightReleased();
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int32_t Delta = GET_WHEEL_DELTA_WPARAM(WParam);
        Mouse.OnWheelDelta(Delta);
        break;
    }

    default:
        break;
    }
    return DefWindowProc(HWnd, Msg, WParam, LParam);
}
