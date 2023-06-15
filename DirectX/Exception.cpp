#include "Exception.h"

#include <sstream>

FException::FException(int32_t Line, const FChar* File) noexcept : Line(Line), File(File) {}

const FChar* FException::What() const noexcept
{
    FOStringStream OStringStream;

    OStringStream << GetType() << "\n" << GetOriginString();
    WhatBuffer = OStringStream.str();
    return WhatBuffer.c_str();
}

const FChar* FException::GetType() const noexcept
{
    return TEXT("FException");
}

int32_t FException::GetLine() const noexcept
{
    return Line;
}

const FString FException::GetFile() const noexcept
{
    return File;
}

FString FException::GetOriginString() const noexcept
{
    FOStringStream OStringStream;
    OStringStream << TEXT("[File] ") << File << "\n" << TEXT("[Line] ") << Line << "\n";
    return OStringStream.str();
}
