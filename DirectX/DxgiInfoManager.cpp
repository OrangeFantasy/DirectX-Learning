#include "DxgiInfoManager.h"
#include "Direct3DException.h"

#pragma comment(lib, "dxguid.lib")

FDxgiInfoManager::FDxgiInfoManager()
{
    // Define function signature of DXGIGetDebugInterface.
    using DXGIGetDebugInterface = HRESULT(WINAPI*)(REFIID, void**);

    // Load the dll that contains the function DXGIGetDebugInterface.
    const HMODULE ModuleDxgiDebug = LoadLibraryEx(TEXT("dxgidebug.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (ModuleDxgiDebug == nullptr)
    {
        throw WIN32_LAST_EXCEPT();
    }

    // Get address of DXGIGetDebugInterface in dll.
    const DXGIGetDebugInterface DxgiGetDebugInference =
        reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(ModuleDxgiDebug, "DXGIGetDebugInterface")));
    if (DxgiGetDebugInference == nullptr)
    {
        throw WIN32_LAST_EXCEPT();
    }

    D3D_CHECK_FAILED_NOINFO(DxgiGetDebugInference(__uuidof(IDXGIInfoQueue), &DxgiInfoQueue));
}

void FDxgiInfoManager::Set() noexcept
{
    // set the index (next) so that the next all to GetMessages() will only get errors generated after this call.
    Next = DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

TArray<FString> FDxgiInfoManager::GetMessages() const
{
    TArray<FString> Messages;
    const UINT64    End = DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for (UINT64 Index = Next; Index < End; ++Index)
    {
        HRESULT Result        = 0;
        SIZE_T  MessageLength = 0U;

        // Get the size of message index in bytes.
        D3D_CHECK_FAILED_NOINFO(DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, Index, nullptr, &MessageLength));

        // Allocate memory for message.
        byte*                    Bytes = new byte[MessageLength];
        DXGI_INFO_QUEUE_MESSAGE* Msg   = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(Bytes);

        // Get the message and push its description into the vector.
        D3D_CHECK_FAILED_NOINFO(DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, Index, Msg, &MessageLength));
#ifdef UNICODE
        const char* _CharMsgStr = Msg->pDescription;
        FString MsgStr;
        MsgStr.assign(_CharMsgStr, _CharMsgStr + std::strlen(_CharMsgStr));
#elif
        FString MsgStr = Msg->pDescription;
#endif // UNICODE

        Messages.emplace_back(MsgStr);

        // Release memory.
        delete[] Bytes;
        Bytes = nullptr;
    }
    return Messages;
}
