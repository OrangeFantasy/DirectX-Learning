#include "Mouse.h"

#include "OraeWin.h"

std::pair<int32_t, int32_t> FMouse::GetPos() const noexcept
{
    return {X, Y};
}

int32_t FMouse::GetPosX() const noexcept
{
    return X;
}

int32_t FMouse::GetPosY() const noexcept
{
    return Y;
}

bool FMouse::LeftIsPress() const noexcept
{
    return bLeftIsPressed;
}

bool FMouse::RightIsPress() const noexcept
{
    return bRightIsPressed;
}

bool FMouse::IsInWindow() const noexcept
{
    return bIsInWindow;
}

FMouse::FEvent FMouse::Read() noexcept
{
    if (EventBuffer.size() > 0U)
    {
        FEvent Event = EventBuffer.front();
        EventBuffer.pop();
        return Event;
    }
    return FEvent();
}

bool FMouse::IsEmpty() const noexcept
{
    return EventBuffer.empty();
}

void FMouse::Flush() noexcept
{
    EventBuffer = std::queue<FEvent>();
}

void FMouse::OnMouseMove(int32_t NewX, int32_t NewY) noexcept
{
    X = NewX;
    Y = NewY;
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::Move, *this));
    TrimBuffer();
}

void FMouse::OnLeftPressed() noexcept
{
    bLeftIsPressed = true;
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::LeftPress, *this));
    TrimBuffer();
}

void FMouse::OnLeftReleased() noexcept
{
    bLeftIsPressed = false;
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::LeftRelease, *this));
    TrimBuffer();
}

void FMouse::OnRightPressed() noexcept
{
    bRightIsPressed = true;
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::RightPress, *this));
    TrimBuffer();
}

void FMouse::OnRightReleased() noexcept
{
    bRightIsPressed = false;
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::RightRelease, *this));
    TrimBuffer();
}

void FMouse::OnWheelUp() noexcept
{
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::WheelUp, *this));
    TrimBuffer();
}

void FMouse::OnWheelDown() noexcept
{
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::WheelDown, *this));
    TrimBuffer();
}

void FMouse::OnWheelDelta(int32_t delta) noexcept
{
    WheelDeltaCarry += delta;
    while (WheelDeltaCarry >= WHEEL_DELTA)
    {
        WheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp();
    }
    while (WheelDeltaCarry <= -WHEEL_DELTA)
    {
        WheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown();
    }
}

void FMouse::OnMouseEnterWindow() noexcept
{
    bIsInWindow = true;
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::EnterWindow, *this));
    TrimBuffer();
}

void FMouse::OnMouseLeaveWindow() noexcept
{
    bIsInWindow = false;
    EventBuffer.push(FMouse::FEvent(FMouse::FEvent::EType::LeaveWindow, *this));
    TrimBuffer();
}

void FMouse::TrimBuffer() noexcept
{
    while (EventBuffer.size() > BufferSize)
    {
        EventBuffer.pop();
    }
}
