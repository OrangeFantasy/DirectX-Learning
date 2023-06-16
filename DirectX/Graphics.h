#pragma once

#include "OraeWin.h"
#include <d3d11.h>
#include <wrl.h>

#include "Direct3DException.h"
#include "DxgiInfoManager.h"

class FGraphics
{
    template <typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    FGraphics(HWND WindowHandle);
    FGraphics(const FGraphics&)            = delete;
    FGraphics& operator=(const FGraphics&) = delete;
    ~FGraphics()                           = default;

    void EndFrame();
    void ClearBuffer(float red, float green, float blue) noexcept;

    void DrawTestTriangle(float Angle);

private:
    ComPtr<ID3D11Device>           Device        = nullptr;
    ComPtr<IDXGISwapChain>         SwapChain     = nullptr;
    ComPtr<ID3D11DeviceContext>    DeviceContext = nullptr;
    ComPtr<ID3D11RenderTargetView> RenderTargetView    = nullptr;

#ifndef NDEBUG
    FDxgiInfoManager InfoManager;
#endif // NDBUG
};
