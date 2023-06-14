#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return {x, y};
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::LeftIsPress() const noexcept
{
    return leftIsPressed;
}

bool Mouse::RightIsPress() const noexcept
{
    return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
    if (buffer.size() > 0U)
    {
        Event e = buffer.front();
        buffer.pop();
        return e;
    }
    return Event();
}

bool Mouse::IsEmpty() const noexcept
{
    return buffer.empty();
}

void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newX, int newY) noexcept
{
    x = newX;
    y = newY;
    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
    leftIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::LeftPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
    leftIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::LeftRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
    rightIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::RightPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
    rightIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::RightRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
