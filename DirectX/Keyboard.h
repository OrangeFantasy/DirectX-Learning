#pragma once

#include <queue>
#include <bitset>

#include "String.h"

class FKeyboard
{
    friend class FWindow;

public:
    class FEvent
    {
    public:
        enum class EType
        {
            Press,
            Release,
            Invalid
        };

    public:
        FEvent() : Type(EType::Invalid), Code(0U) {}
        FEvent(EType Ty, uint8_t Code) : Type(Ty), Code(Code) {}

        bool IsPress() const noexcept { return Type == EType::Press; }
        bool IsRelease() const noexcept { return Type == EType::Release; }
        bool IsValid() const noexcept { return Type != EType::Invalid; }
        uint8_t GetCode() const noexcept { return Code; }

    private:
        EType Type;
        uint8_t Code;
    };

public:
    FKeyboard()                            = default;
    FKeyboard(const FKeyboard&)            = delete;
    FKeyboard& operator=(const FKeyboard&) = delete;
    ~FKeyboard()                           = default;

    // Key event stuff.
    bool KeyIsPressed(uint8_t Keycode) const noexcept;
    FEvent ReadKey() noexcept;
    bool KeyIsEmpty() const noexcept;
    void FlushKey() noexcept;

    // Char event stuff.
    FChar ReadChar() noexcept;
    bool CharIsEmpty() const noexcept;
    void FlushChar() noexcept;
    void Flush() noexcept;

    // Autorepeat control.
    void EnableAutorepeat() noexcept;
    void DisableAutorepeat() noexcept;
    bool AutorepeatIsEnable() const noexcept;

private:
    void OnKeyPressed(uint8_t Keycode) noexcept;
    void OnKeyReleased(uint8_t Keycode) noexcept;
    void OnChar(FChar Character) noexcept;
    void ClearState() noexcept;

    template <typename T>
    static void TrimBuffer(std::queue<T>& Buffer) noexcept;

private:
    static constexpr unsigned int NumKeys      = 256U;
    static constexpr unsigned int BufferSize = 16U;

    bool bAutorepeatEnabled = false;

    std::bitset<NumKeys> KeyStates;
    std::queue<FEvent>   KeyBuffer;
    std::queue<FChar>    CharBuffer;
};

template <typename T>
inline void FKeyboard::TrimBuffer(std::queue<T>& Buffer) noexcept
{
    while (Buffer.size() > BufferSize)
    {
        Buffer.pop();
    }
}
