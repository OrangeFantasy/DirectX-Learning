#pragma once

#include "OraeWin.h"
#include "Exception.h"

class FWin32Exception : public FException
{
public:
    FWin32Exception(int Line, const FChar* File, HRESULT Result) noexcept;

    virtual const FChar* What() const noexcept override;
    virtual const FChar* GetType() const noexcept override;

    HRESULT GetErrorCode() const noexcept;
    FString GetErrorString() const noexcept;

    static FString TranslateErrorCode(HRESULT Result);

private:
    HRESULT Result;
};

// Win32 error exception macro.
#define WIN32_EXCEPT(Result) FWin32Exception(__LINE__, TEXT(__FILE__), Result)
#define WIN32_LAST_EXCEPT() FWin32Exception(__LINE__, TEXT(__FILE__), GetLastError())
