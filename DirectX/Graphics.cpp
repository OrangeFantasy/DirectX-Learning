#include "Graphics.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

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
        D3D_DRIVER_TYPE_HARDWARE,                           //
        nullptr,                                            //
        SwapChainCreateFlags,                               //
        nullptr,                                            //
        0,                                                  //
        D3D11_SDK_VERSION,                                  //
        &SwapChainDesc,                                     //
        &SwapChain,                                         //
        &Device,                                            //
        nullptr,                                            //
        &DeviceContext                                      //
        ));

    // Gain access to texture subresource in swap chain (back buffer).
    ComPtr<ID3D11Resource> BackBuffer = nullptr;
    D3D_CHECK_FAILED(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBuffer));
    D3D_CHECK_FAILED(Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &RenderTargetView));
}

void FGraphics::EndFrame()
{
    if (HRESULT Result = SwapChain->Present(1U, 0U); FAILED(Result))
    {
        throw Result == DXGI_ERROR_DEVICE_REMOVED ? D3D_DEVICE_REMOVED_EXCEPT(Device->GetDeviceRemovedReason()) : D3D_EXCEPT(Result);
    }
}

void FGraphics::ClearBuffer(float Red, float Green, float Blue) noexcept
{
    const float Color[] = {Red, Green, Blue, 1.0f};
    DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), Color);
}

void FGraphics::DrawTestTriangle()
{
    struct Vertex
    {
        float X;
        float Y;
    };

    // Create vertex buffer.
    const Vertex Vertices[] = {{0.0f, 0.75f}, {0.5f, -0.5f}, {-0.5f, -0.5f}};

    D3D11_BUFFER_DESC BufferDesc   = {};
    BufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    BufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    BufferDesc.CPUAccessFlags      = 0U;
    BufferDesc.MiscFlags           = 0U;
    BufferDesc.ByteWidth           = sizeof(Vertices);
    BufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA Data = {};
    Data.pSysMem                = Vertices;

    ComPtr<ID3D11Buffer> VertexBuffer;
    D3D_CHECK_FAILED(Device->CreateBuffer(&BufferDesc, &Data, &VertexBuffer));

    // Bind vertex b uffer to pipeline.
    const UINT Stride = sizeof(Vertex);
    const UINT Offset = 0U;
    DeviceContext->IASetVertexBuffers(0U, 1U, VertexBuffer.GetAddressOf(), &Stride, &Offset);

    // Create vertex shader.
    ComPtr<ID3DBlob>           Blob;
    ComPtr<ID3D11VertexShader> VertexShader;
    D3D_CHECK_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &Blob));
    D3D_CHECK_FAILED(Device->CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &VertexShader));

    // Bind vertex shader to pipeline.
    DeviceContext->VSSetShader(VertexShader.Get(), nullptr, 0U);

    // Create input (vertex) layout (2d position only).
    ComPtr<ID3D11InputLayout>      InputLayout;
    const D3D11_INPUT_ELEMENT_DESC InputElementDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} // Same to VertexShader input.
    };
    UINT NumElements = sizeof(InputElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);
    Device->CreateInputLayout(InputElementDesc, NumElements, Blob->GetBufferPointer(), Blob->GetBufferSize(), &InputLayout);

    //Bind input layout to pipeline.
    DeviceContext->IASetInputLayout(InputLayout.Get());
    
    // Create pixel shader.
    ComPtr<ID3D11PixelShader> PixelShader;
    D3D_CHECK_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &Blob));
    D3D_CHECK_FAILED(Device->CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &PixelShader));

    // Bind pixel shader to pipeline.
    DeviceContext->PSSetShader(PixelShader.Get(), nullptr, 0U);

    // Bind render target to pipeline.
    DeviceContext->OMSetRenderTargets(1U, RenderTargetView.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (group of 3 vertices).
    DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Configure viewport.
    D3D11_VIEWPORT ViewPort = {};
    ViewPort.Width          = 800;
    ViewPort.Height         = 600;
    ViewPort.MinDepth       = 0;
    ViewPort.MaxDepth       = 1;
    ViewPort.TopLeftX       = 0;
    ViewPort.TopLeftY       = 0;
    DeviceContext->RSSetViewports(1U, &ViewPort);

    // Draw.
    UINT NumVertices = sizeof(Vertices) / sizeof(Vertex);
    DeviceContext->Draw(NumVertices, 0U);
}
