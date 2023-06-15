#pragma once

#include "Win32Exception.h"
#include "Array.h"

class FDirect3DException : public FWin32Exception
{
public:
    FDirect3DException(int Line, const FChar* File, HRESULT Result, TArray<FString> Messages = {}) noexcept;

    virtual const FChar* GetType() const noexcept override;
    virtual const FChar* What() const noexcept override;

    FString GetErrorInfo() const noexcept;

private:
    FString Info;
};

class FNoGraphicsException : public FWin32Exception
{
public:
    using FWin32Exception::FWin32Exception;

    virtual const FChar* GetType() const noexcept override;
};

#define D3D_CHECK_FAILED_NOINFO(Result)                                                                                                    \
    if (HRESULT _Result = Result; FAILED(_Result))                                                                                         \
    {                                                                                                                                      \
        throw FDirect3DException(__LINE__, TEXT(__FILE__), Result);                                                                        \
    }

#ifndef NDEBUG
#define D3D_EXCEPT(Result) FDirect3DException(__LINE__, TEXT(__FILE__), Result, InfoManager.GetMessages())
#define D3D_CHECK_FAILED(Result)                                                                                                           \
    InfoManager.Set();                                                                                                                     \
    if (HRESULT _Result = Result; FAILED(_Result))                                                                                         \
    {                                                                                                                                      \
        throw D3D_EXCEPT(_Result);                                                                                                         \
    }
#define D3D_DEVICE_REMOVED_EXCEPT(Result) D3D_EXCEPT(Result)
#else
#define D3D_EXCEPT(Result) FDirect3DException(__LINE__, TEXT(__FILE__), Result)
#define D3D_CHECK_FAILED(Result) D3D_CHECK_FAILED_NOINFO(Result)
#define D3D_DEVICE_REMOVED_EXCEPT(Result) D3D_EXCEPT(Result)
#endif

#define NO_GRAPHICS_EXCEPT() FNoGraphicsException(__LINE__, TEXT(__FILE__), 1)
