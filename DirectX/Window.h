#pragma once

#include <optional>

#include "OraeWin.h"
#include "Win32Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class FWindow
{
private:
    // WindowClass.
    class FWindowClass
    {
    public:
        static const FChar* GetName() noexcept;
        static HINSTANCE    GetInstance() noexcept;

    private:
        FWindowClass() noexcept;
        ~FWindowClass();
        FWindowClass(const FWindowClass&)            = delete;
        FWindowClass& operator=(const FWindowClass&) = delete;

    private:
        static constexpr const FChar* WindowClassName = TEXT("DirectX3D Window");
        static FWindowClass WindowClass;

        HINSTANCE Instance;
    };

public:
    FWindow(int32_t Width, int32_t Height, const FChar* wndName);
    FWindow(const FWindow&)            = delete;
    FWindow& operator=(const FWindow&) = delete;
    ~FWindow();

    static std::optional<int32_t> ProcessMessages();

    void SetTitle(const FString Title);

    FGraphics& Graphics();

private:
    static LRESULT WINAPI HandleMsgSetup(_In_ HWND HWnd, _In_ UINT Msg, _In_ WPARAM WParam, _In_ LPARAM LParam) noexcept;
    static LRESULT WINAPI HandleMsgThunk(_In_ HWND HWnd, _In_ UINT Msg, _In_ WPARAM WParam, _In_ LPARAM LParam) noexcept;

    LRESULT HandleMsg(HWND WindowHandle, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
    FKeyboard Keyboard;
    FMouse    Mouse;

private:
    int32_t Width;
    int32_t Height;
    HWND WindowHandle;

    FGraphics* GraphicsPtr = nullptr;
};
