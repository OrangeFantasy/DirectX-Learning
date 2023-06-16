#pragma once

#include <vector>
#include <dxgidebug.h>
#include <wrl.h>

#include "OraeWin.h"
#include "Array.h"
#include "String.h"

class FDxgiInfoManager
{
public:
    FDxgiInfoManager();
    FDxgiInfoManager(const FDxgiInfoManager&) = delete;
    ~FDxgiInfoManager() = default;
    FDxgiInfoManager operator=(const FDxgiInfoManager&) = delete;

    void Set() noexcept;

    TArray<FString> GetMessages() const;

private:
    UINT64 Next = 0U;

    Microsoft::WRL::ComPtr<struct IDXGIInfoQueue> DxgiInfoQueue = nullptr;
};
