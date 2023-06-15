#pragma once

#include <vector>

#include "OraeWin.h"
#include "Array.h"
#include "String.h"

class FDxgiInfoManager
{
public:
    FDxgiInfoManager();
    FDxgiInfoManager(const FDxgiInfoManager&) = delete;
    ~FDxgiInfoManager();
    FDxgiInfoManager operator=(const FDxgiInfoManager&) = delete;

    void Set() noexcept;

    TArray<FString> GetMessages() const;

private:
    UINT64 Next = 0U;

    struct IDXGIInfoQueue* DxgiInfoQueue = nullptr;
};
