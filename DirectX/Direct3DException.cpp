#include "Direct3DException.h"

FDirect3DException::FDirect3DException(int Line, const FChar* File, HRESULT Result, TArray<FString> Messages) noexcept
    : FWin32Exception(Line, File, Result)
{
    // Join all info messages with newlines into single string
    for (const FString& Msg : Messages)
    {
        Info += Msg;
        Info.push_back('\n');
    }
    // Remove final newline if exists.
    if (!Info.empty())
    {
        Info.pop_back();
    }
}

const FChar* FDirect3DException::GetType() const noexcept
{
    return TEXT("Direct3D Exception.");
}

const FChar* FDirect3DException::What() const noexcept
{
    FOStringStream OStringStream;
    OStringStream << GetType() << "\n"
                  << TEXT("[Error Code] ") << GetErrorCode() << "\n"
                  << TEXT("[Desription] ") << GetErrorString() << "\n";
    if (!Info.empty())
    {
        OStringStream << TEXT("[Error Info] ") << GetErrorInfo() << "\n";
    }
    OStringStream << "\n" << GetOriginString();
    WhatBuffer = OStringStream.str();
    return WhatBuffer.c_str();
}

FString FDirect3DException::GetErrorInfo() const noexcept
{
    return Info;
}

const FChar* FNoGraphicsException::GetType() const noexcept
{
    return TEXT("No Graphics Exception.");
}
