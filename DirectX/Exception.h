#pragma once

#include <exception>

#include "String.h"

class FException : public std::exception
{
public:
    FException(int32_t Line, const FChar* File) noexcept;

    virtual const FChar* What() const noexcept;
    virtual const FChar* GetType() const noexcept;

    int32_t         GetLine() const noexcept;
    const FString   GetFile() const noexcept;
    FString         GetOriginString() const noexcept;

private:
    virtual const char* what() const { return ""; }

protected:
    mutable FString WhatBuffer;

private:
    int32_t Line;
    FString File;
};
