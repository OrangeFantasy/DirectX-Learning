#include "Keyboard.h"

bool FKeyboard::KeyIsPressed(uint8_t Keycode) const noexcept
{
    return KeyStates[Keycode];
}

FKeyboard::FEvent FKeyboard::ReadKey() noexcept
{
    if (KeyBuffer.size() > 0U)
    {
        FKeyboard::FEvent Event = KeyBuffer.front();
        KeyBuffer.pop();
        return Event;
    }
    return FEvent();
}

bool FKeyboard::KeyIsEmpty() const noexcept
{
    return KeyBuffer.empty();
}

void FKeyboard::FlushKey() noexcept
{
    KeyBuffer = std::queue<FEvent>();
}

FChar FKeyboard::ReadChar() noexcept
{
    if (CharBuffer.size() > 0U)
    {
        FChar CharCode = CharBuffer.front();
        CharBuffer.pop();
        return CharCode;
    }
    return 0U;
}

bool FKeyboard::CharIsEmpty() const noexcept
{
    return CharBuffer.empty();
}

void FKeyboard::FlushChar() noexcept
{
    CharBuffer = std::queue<FChar>();
}

void FKeyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void FKeyboard::EnableAutorepeat() noexcept
{
    bAutorepeatEnabled = true;
}

void FKeyboard::DisableAutorepeat() noexcept
{
    bAutorepeatEnabled = false;
}

bool FKeyboard::AutorepeatIsEnable() const noexcept
{
    return bAutorepeatEnabled;
}

void FKeyboard::OnKeyPressed(uint8_t Keycode) noexcept
{
    KeyStates[Keycode] = true;
    KeyBuffer.push(FKeyboard::FEvent(FKeyboard::FEvent::EType::Press, Keycode));
    TrimBuffer(KeyBuffer);
}

void FKeyboard::OnKeyReleased(uint8_t Keycode) noexcept
{
    KeyStates[Keycode] = false;
    KeyBuffer.push(FKeyboard::FEvent(FKeyboard::FEvent::EType::Release, Keycode));
    TrimBuffer(KeyBuffer);
}

void FKeyboard::OnChar(FChar Character) noexcept
{
    CharBuffer.push(Character);
    TrimBuffer(CharBuffer);
}

void FKeyboard::ClearState() noexcept
{
    KeyStates.reset();
}
