#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            Press,
            Release,
            Invalid
        };

    public:
        Event() : type(Type::Invalid), code(0U) {}
        Event(Type _type, unsigned char _code) : type(_type), code(_code) {}

        bool IsPress() const noexcept { return type == Type::Press; }
        bool IsRelease() const noexcept { return type == Type::Release; }
        bool IsValid() const noexcept { return type != Type::Invalid; }

        unsigned char GetCode() const noexcept { return code; }

    private:
        Type          type;
        unsigned char code;
    };

public:
    Keyboard()                           = default;
    Keyboard(const Keyboard&)            = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    // Key event stuff.
    bool  KeyIsPressed(unsigned char keycode) const noexcept;
    Event ReadKey() noexcept;
    bool  KeyIsEmpty() const noexcept;
    void  FlushKey() noexcept;

    // Char event stuff.
    char ReadChar() noexcept;
    bool CharIsEmpty() const noexcept;
    void FlushChar() noexcept;
    void Flush() noexcept;

    // Autorepeat control.
    void EnableAutorepeat() noexcept;
    void DisableAutorepeat() noexcept;
    bool AutorepeatIsEnable() const noexcept;

private:
    void OnKeyPressed(unsigned char keycode) noexcept;
    void OnKeyReleased(unsigned char keycode) noexcept;
    void OnChar(char character) noexcept;
    void ClearState() noexcept;

    template <typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
    static constexpr unsigned int nKeys      = 256U;
    static constexpr unsigned int bufferSize = 16U;

    bool autorepeatEnabled = false;

    std::bitset<nKeys> keyStates;
    std::queue<Event>  keyBuffer;
    std::queue<char>   charBuffer;
};

template <typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
