#include "Win32Exception.h"

FWin32Exception::FWin32Exception(int Line, const FChar* File, HRESULT Result) noexcept : FException(Line, File), Result(Result) {}

const FChar* FWin32Exception::What() const noexcept
{
    FOStringStream OStringStream;
    OStringStream << GetType() << "\n"
                  << TEXT("[Error Code] ") << GetErrorCode() << "\n"
                  << TEXT("[Desription] ") << GetErrorString() << "\n"
                  << GetOriginString();
    WhatBuffer = OStringStream.str();
    return WhatBuffer.c_str();
}

const FChar* FWin32Exception::GetType() const noexcept
{
    return TEXT("Win32 Exception");
}

HRESULT FWin32Exception::GetErrorCode() const noexcept
{
    return Result;
}

FString FWin32Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(Result);
}

FString FWin32Exception::TranslateErrorCode(HRESULT Result)
{
    FChar* MsgBuffer = nullptr;
    DWORD MsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, //
        nullptr,                                                                                                              //
        Result,                                                                                                               //
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                                                                            //
        reinterpret_cast<LPWSTR>(&MsgBuffer),                                                                                 //
        0,                                                                                                                    //
        nullptr);
    if (MsgLen == 0)
    {
        return TEXT("Unidentfied error Code");
    }
    FString ErrorString = MsgBuffer;
    LocalFree(MsgBuffer);
    return ErrorString;
}
