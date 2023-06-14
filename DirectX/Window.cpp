#include "Window.h"

#include <sstream>

#include "resource.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc    = {0};
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = HandleMsgSetup;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetInstance();
    wc.hIcon         = LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor       = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm       = nullptr;

    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

const char_t* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInstance;
}

Window::Window(int nWidth, int nHeight, const char_t* wndName) : width(nWidth), height(nHeight)
{
    // Calculate window size based on desired client region size
    RECT wr   = {};
    wr.left   = 100;
    wr.right  = width + wr.left;
    wr.top    = 100;
    wr.bottom = height + wr.top;
    if (!AdjustWindowRectEx(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE, 0))
    {
        throw HWND_LAST_EXCEPT();
    }

    // Create window & Get hWnd
    hWnd = CreateWindow(WindowClass::GetName(),   //
        wndName,                                  //
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //
        CW_USEDEFAULT,                            //
        CW_USEDEFAULT,                            //
        wr.right - wr.left,                       //
        wr.bottom - wr.top,                       //
        nullptr,                                  //
        nullptr,                                  //
        WindowClass::GetInstance(),               //
        this);
    if (hWnd == nullptr)
    {
        throw HWND_LAST_EXCEPT();
    }

    // Show window.
    ShowWindow(hWnd, SW_SHOW);
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

void Window::SetTitle(const string_t title)
{
    if (SetWindowText(hWnd, title.c_str()) == 0)
    {
        throw HWND_LAST_EXCEPT();
    }
}

LRESULT WINAPI Window::HandleMsgSetup(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
{
    // Use create parameter passed in from CreateWindow() to store window class pointer.
    if (Msg == WM_NCCREATE)
    {
        // Extract ptr to window class from creation data.
        const CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window*             pWnd    = static_cast<Window*>(pCreate->lpCreateParams);

        // Set WinAPI-managed user data to store ptr to window class.
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

        // Set message proc to normal (non-setup) handler now that setup is finished.
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        // Forward message to window class handler.
        return pWnd->HandleMsg(hWnd, Msg, wParam, lParam);
    }
    // If we get a messgae before the WM_NCCREATE message, handle with default handler.
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
{
    // Retrieve ptr to window class.
    Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    // Forward message to window class handler.
    return pWnd->HandleMsg(hWnd, Msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (Msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:
        keyboard.ClearState();
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (!(lParam & 0x40000000) || keyboard.AutorepeatIsEnable())
        {
            keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        keyboard.OnChar(static_cast<unsigned char>(wParam));
        break;

    case WM_MOUSEMOVE:
    {
        POINTS pt = MAKEPOINTS(lParam);
        // In window.
        if (pt.x >= 0 && pt.x < width && pt.y > 0 && pt.y < height)
        {
            mouse.OnMouseMove(pt.x, pt.y);
            if (!mouse.IsInWindow())
            {
                SetCapture(hWnd);
                mouse.OnMouseEnterWindow();
            }
        }
        // Not in window -> log move/maintain capture if button down.
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse.OnMouseMove(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture();
                mouse.OnMouseLeaveWindow();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        POINTS pt    = MAKEPOINTS(lParam);
        int    delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(pt.x, pt.y, delta);
        break;
    }

    default:
        break;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

Window::Exception::Exception(int line, const char_t* file, HRESULT hResult) noexcept : OraeException(line, file), hResult(hResult) {}

const char_t* Window::Exception::What() const noexcept
{
#ifdef UNICODE
    std::wostringstream oss;
    oss << GetType() << "\n"
        << "[Error Code] " << GetErrorCode() << "\n"
        << "[Desription] " << GetErrorString() << "\n"
        << GetOriginString();
#elif
    std::ostringstream oss;
    oss << GetType() << L"\n"
        << L"[Error Code] " << GetErrorCode() << L"\n"
        << L"[Desription] " << GetErrorString() << L"\n"
        << GetOriginString();
#endif

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char_t* Window::Exception::GetType() const noexcept
{
    return TEXT("Orae Window Exception");
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
    return hResult;
}

string_t Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(hResult);
}

string_t Window::Exception::TranslateErrorCode(HRESULT hResult)
{
    char_t* pMsgBuffer = nullptr;
    DWORD   nMsgLen    = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, //
             nullptr,                                                                                                               //
             hResult,                                                                                                               //
             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                                                                             //
             reinterpret_cast<LPWSTR>(&pMsgBuffer),                                                                                 //
             0,                                                                                                                     //
             nullptr);
    if (nMsgLen == 0)
    {
        return TEXT("Unidentfied error code");
    }
    string_t errorString = pMsgBuffer;
    LocalFree(pMsgBuffer);
    return errorString;
}
