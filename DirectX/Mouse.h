#pragma once

#include <queue>

class Mouse
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            LeftPress,
            LeftRelease,
            RightPress,
            RightRelease,
            WheelUp,
            WheelDown,
            Move,
            Invalid
        };

    public:
        Event() noexcept : type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), x(0), y(0) {}
        Event(Type _type, const Mouse& parent) noexcept
            : type(_type), leftIsPressed(parent.leftIsPressed), rightIsPressed(parent.rightIsPressed), x(parent.x), y(parent.y)
        {
        }

        bool LeftIsPress() const noexcept { return leftIsPressed; }
        bool RightIsPress() const noexcept { return rightIsPressed; }
        bool isValid() const noexcept { return type != Type::Invalid; }
        Type GetType() const noexcept { return type; }

        std::pair<int, int> GetPos() const noexcept { return {x, y}; }

        int GetPosX() const noexcept { return x; }
        int GetPosY() const noexcept { return y; }

    private:
        Type type;
        bool leftIsPressed;
        bool rightIsPressed;
        int  x;
        int  y;
    };

public:
    Mouse()                        = default;
    Mouse(const Mouse&)            = delete;
    Mouse& operator=(const Mouse&) = delete;

    std::pair<int, int> GetPos() const noexcept;

    int GetPosX() const noexcept;
    int GetPosY() const noexcept;

    bool LeftIsPress() const noexcept;
    bool RightIsPress() const noexcept;

    Mouse::Event Read() noexcept;

    bool IsEmpty() const noexcept;
    void Flush() noexcept;

private:
    void OnMouseMove(int x, int y) noexcept;
    void OnLeftPressed(int x, int y) noexcept;
    void OnLeftReleased(int x, int y) noexcept;
    void OnRightPressed(int x, int y) noexcept;
    void OnRightReleased(int x, int y) noexcept;
    void OnWheelUp(int x, int y) noexcept;
    void OnWheelDown(int x, int y) noexcept;
    void TrimBuffer() noexcept;

private:
    static constexpr unsigned int bufferSize = 16U;

    int x;
    int y;
    bool leftIsPressed = false;
    bool rightIsPressed = false;

    std::queue<Event> buffer;
};
