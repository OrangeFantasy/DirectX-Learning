#pragma once

#include "OraeWin.h"
#include "OraeException.h"

#include "Keyboard.h"
#include "Mouse.h"

#ifdef UNICODE
using char_t   = wchar_t;
using string_t = std::wstring;
#elif
using char_t   = char;
using string_t = std::string;
#endif

class Window
{
private:
    // Exception.
    class Exception : public OraeException
    {
    public:
        Exception(int line, const char_t* file, HRESULT hResult) noexcept;

        virtual const char_t* What() const noexcept override;
        virtual const char_t* GetType() const noexcept override;

        HRESULT  GetErrorCode() const noexcept;
        string_t GetErrorString() const noexcept;

        static string_t TranslateErrorCode(HRESULT hResult);

    private:
        HRESULT hResult;
    };

    // WindowClass.
    class WindowClass
    {
    public:
        static const char_t* GetName() noexcept;
        static HINSTANCE     GetInstance() noexcept;

    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&)            = delete;
        WindowClass& operator=(const WindowClass&) = delete;

    private:
        static constexpr const char_t* wndClassName = TEXT("Direct3D Window");
        static WindowClass             wndClass;

        HINSTANCE hInstance;
    };

public:
    Window(int nWidth, int nHeight, const char_t* wndName);
    ~Window();
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    void SetTitle(const string_t title);

private:
    static LRESULT WINAPI HandleMsgSetup(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept;
    static LRESULT WINAPI HandleMsgThunk(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept;

    LRESULT HandleMsg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
    Keyboard keyboard;
    Mouse    mouse;

private:
    int  width;
    int  height;
    HWND hWnd;
};

// Error exception macro.
#define HWND_EXCEPT(hResult) Window::Exception(__LINE__, TEXT(__FILE__), hResult)
#define HWND_LAST_EXCEPT() Window::Exception(__LINE__, TEXT(__FILE__), GetLastError())