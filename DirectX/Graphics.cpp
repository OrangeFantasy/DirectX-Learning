#include "Graphics.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

constexpr const float WindowHeight = 600.0f;
constexpr const float WindowWidth = 800.0f;
constexpr const float AspectRatio  = WindowHeight / WindowWidth;

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

void FGraphics::DrawTestTriangle(float Angle)
{
    struct FVertex
    {
        struct
        {
            float X;
            float Y;
        } FPosition;

        struct
        {
            uint8_t R;
            uint8_t G;
            uint8_t B;
            uint8_t A;
        } FColor;
    };

    // Create vertex buffer.
    const FVertex Vertices[] = {
        {0.0f, 1.0f, 255, 255, 0, 0}, //
        {0.5f, -0.5f, 0, 255, 0, 255}, //
        {-0.5f, -0.5f, 0, 0, 255, 0},  //
        {-0.3f, 0.3f, 0, 255, 0, 0},   //
        {0.3f, 0.3f, 0, 0, 255, 0},    //
        {0.0f, -0.8f, 255, 0, 0, 0}    //
    };

    D3D11_BUFFER_DESC VertexBufferDesc   = {};
    VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.CPUAccessFlags      = 0U;
    VertexBufferDesc.MiscFlags           = 0U;
    VertexBufferDesc.ByteWidth           = sizeof(Vertices);
    VertexBufferDesc.StructureByteStride = sizeof(FVertex);

    D3D11_SUBRESOURCE_DATA VertexData = {};
    VertexData.pSysMem                = Vertices;

    ComPtr<ID3D11Buffer> VertexBuffer;
    D3D_CHECK_FAILED(Device->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer));

    // Bind vertex buffer to pipeline.
    const UINT Stride = sizeof(FVertex);
    const UINT Offset = 0U;
    DeviceContext->IASetVertexBuffers(0U, 1U, VertexBuffer.GetAddressOf(), &Stride, &Offset);

    // Create index buffer.
    const uint16_t Indices[] = {
        0, 1, 2, //
        0, 2, 3, //
        0, 4, 1, //
        2, 1, 5  //
    };

    D3D11_BUFFER_DESC IndexBufferDesc   = {};
    IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    IndexBufferDesc.CPUAccessFlags      = 0U;
    IndexBufferDesc.MiscFlags           = 0U;
    IndexBufferDesc.ByteWidth           = sizeof(Indices);
    IndexBufferDesc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA IndexData = {};
    IndexData.pSysMem                = Indices;

    ComPtr<ID3D11Buffer> IndexBuffer;
    D3D_CHECK_FAILED(Device->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer));

    // Bind index buffer to pipeline.
    DeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0U);
    
    // Create shader constant buffer.
    struct FConstantMatrix
    {
        struct
        {
            float Element[4][4];
        } FTransformation;
    };
    const FConstantMatrix ConstantMatrix[] = {
        {
            std::cos(Angle) * AspectRatio,  std::sin(Angle), 0.0f, 0.0f, //
            -std::sin(Angle) * AspectRatio, std::cos(Angle), 0.0f, 0.0f, //
            0.0f,                           0.0f,            1.0f, 0.0f, //
            0.0f,                           0.0f,            0.0f, 1.0f  //
        }                                                                //
    };

    D3D11_BUFFER_DESC ConstantBufferDesc   = {};
    ConstantBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    ConstantBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
    ConstantBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    ConstantBufferDesc.MiscFlags           = 0U;
    ConstantBufferDesc.ByteWidth           = sizeof(ConstantMatrix);
    ConstantBufferDesc.StructureByteStride = 0U;

    D3D11_SUBRESOURCE_DATA ConstantData = {};
    ConstantData.pSysMem                = ConstantMatrix;

    ComPtr<ID3D11Buffer> ConstantBuffer;
    D3D_CHECK_FAILED(Device->CreateBuffer(&ConstantBufferDesc, &ConstantData, &ConstantBuffer));

    // Bind constant buffer to pipeline.
    DeviceContext->VSSetConstantBuffers(0U, 1U, ConstantBuffer.GetAddressOf());
   
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
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, //
        {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8U, D3D11_INPUT_PER_VERTEX_DATA, 0}  //
    };
    UINT NumElements = sizeof(InputElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);
    Device->CreateInputLayout(InputElementDesc, NumElements, Blob->GetBufferPointer(), Blob->GetBufferSize(), &InputLayout);

    // Bind input layout to pipeline.
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
    ViewPort.Width    = WindowWidth;
    ViewPort.Height   = WindowHeight;
    ViewPort.MinDepth = 0.0f;
    ViewPort.MaxDepth = 1.0f;
    ViewPort.TopLeftX = 0.0f;
    ViewPort.TopLeftY = 0.0f;
    DeviceContext->RSSetViewports(1U, &ViewPort);

    // Draw.
    UINT NumIndices = sizeof(Indices) / sizeof(uint16_t);
    DeviceContext->DrawIndexed(NumIndices, 0U, 0U);
}
