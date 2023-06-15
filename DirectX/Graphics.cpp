#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

FGraphics::FGraphics(HWND WindowHandle)
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};

    SwapChainDesc.BufferDesc.Width                   = 0;
    SwapChainDesc.BufferDesc.Height                  = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator   = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    SwapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChainDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

    SwapChainDesc.SampleDesc.Count   = 1;
    SwapChainDesc.SampleDesc.Quality = 0;

    SwapChainDesc.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount  = 1;
    SwapChainDesc.OutputWindow = WindowHandle;
    SwapChainDesc.Windowed     = TRUE;
    SwapChainDesc.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
    SwapChainDesc.Flags        = 0;

    UINT SwapChainCreateFlags = 0U;
#ifndef NDEBUG
    SwapChainCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG

    // Create device, front/back buffers, swap chain and rendering context.
    D3D_CHECK_FAILED(D3D11CreateDeviceAndSwapChain(nullptr, //
        D3D_DRIVER_TYPE_HARDWARE,                                //
        nullptr,                                                 //
        SwapChainCreateFlags,                                    //
        nullptr,                                                 //
        0,                                                       //
        D3D11_SDK_VERSION,                                       //
        &SwapChainDesc,                                          //
        &SwapChain,                                              //
        &Device,                                                 //
        nullptr,                                                 //
        &DeviceContext                                           //
        ));

    // Gain access to texture subresource in swap chain (back buffer).
    ID3D11Resource* BackBuffer = nullptr;
    D3D_CHECK_FAILED(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer)));
    D3D_CHECK_FAILED(Device->CreateRenderTargetView(BackBuffer, nullptr, &TargetView));
    BackBuffer->Release();
}

FGraphics::~FGraphics()
{
    if (Device != nullptr)
    {
        Device->Release();
    }
    if (SwapChain != nullptr)
    {
        SwapChain->Release();
    }
    if (DeviceContext != nullptr)
    {
        DeviceContext->Release();
    }
    if (TargetView != nullptr)
    {
        TargetView->Release();
    }
}

void FGraphics::EndFrame()
{
    if (HRESULT Result = SwapChain->Present(1U, 0U); FAILED(Result))
    {
        throw Result == DXGI_ERROR_DEVICE_REMOVED ? D3D_DEVICE_REMOVED_EXCEPT(Device->GetDeviceRemovedReason())
                                                  : D3D_EXCEPT(Result);
    }
}

void FGraphics::ClearBuffer(float Red, float Green, float Blue) noexcept
{
    const float Color[] = {Red, Green, Blue, 1.0f};
    DeviceContext->ClearRenderTargetView(TargetView, Color);
}
