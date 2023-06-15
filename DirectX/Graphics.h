#pragma once

#include "OraeWin.h"
#include <d3d11.h>

#include "Direct3DException.h"
#include "DxgiInfoManager.h"

class FGraphics
{
public:
    FGraphics(HWND WindowHandle);
    FGraphics(const FGraphics&)            = delete;
    FGraphics& operator=(const FGraphics&) = delete;
    ~FGraphics();

    void EndFrame();
    void ClearBuffer(float red, float green, float blue) noexcept;

private:
    ID3D11Device*           Device        = nullptr;
    IDXGISwapChain*         SwapChain     = nullptr;
    ID3D11DeviceContext*    DeviceContext = nullptr;
    ID3D11RenderTargetView* TargetView    = nullptr;
    
#ifndef NDEBUG
    FDxgiInfoManager InfoManager;
#endif // NDBUG
};
