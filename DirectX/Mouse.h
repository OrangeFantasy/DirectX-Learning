#pragma once

#include <queue>

class FMouse
{
    friend class FWindow;

public:
    class FEvent
    {
    public:
        enum class EType
        {
            LeftPress,
            LeftRelease,
            RightPress,
            RightRelease,
            WheelUp,
            WheelDown,
            Move,
            EnterWindow,
            LeaveWindow,
            Invalid
        };

    public:
        FEvent() noexcept : Type(EType::Invalid), bLeftIsPressed(false), bRightIsPressed(false), X(0), Y(0) {}
        FEvent(EType Ty, const FMouse& Parent) noexcept
            : Type(Ty), bLeftIsPressed(Parent.bLeftIsPressed), bRightIsPressed(Parent.bRightIsPressed), X(Parent.X), Y(Parent.Y)
        {}

        bool LeftIsPress() const noexcept { return bLeftIsPressed; }
        bool RightIsPress() const noexcept { return bRightIsPressed; }
        bool IsValid() const noexcept { return Type != EType::Invalid; }
        EType GetType() const noexcept { return Type; }

        std::pair<int32_t, int32_t> GetPos() const noexcept { return {X, Y}; }
        int32_t GetPosX() const noexcept { return X; }
        int32_t GetPosY() const noexcept { return Y; }

    private:
        EType Type;

        int32_t X;
        int32_t Y;
        bool bLeftIsPressed;
        bool bRightIsPressed;
    };

public:
    FMouse()                         = default;
    FMouse(const FMouse&)            = delete;
    FMouse& operator=(const FMouse&) = delete;
    ~FMouse()                        = default;

    std::pair<int32_t, int32_t> GetPos() const noexcept;
    int32_t GetPosX() const noexcept;
    int32_t GetPosY() const noexcept;

    bool LeftIsPress() const noexcept;
    bool RightIsPress() const noexcept;
    bool IsInWindow() const noexcept;
    bool IsEmpty() const noexcept;

    FMouse::FEvent Read() noexcept;
    void Flush() noexcept;

private:
    void OnMouseMove(int32_t NewX, int32_t NewY) noexcept;
    void OnLeftPressed() noexcept;
    void OnLeftReleased() noexcept;
    void OnRightPressed() noexcept;
    void OnRightReleased() noexcept;
    void OnWheelUp() noexcept;
    void OnWheelDown() noexcept;
    void OnWheelDelta(int32_t Delta) noexcept;
    void OnMouseEnterWindow() noexcept;
    void OnMouseLeaveWindow() noexcept;
    void TrimBuffer() noexcept;

private:
    static constexpr int32_t BufferSize = 16;

    int32_t X = 0;
    int32_t Y = 0;
    int32_t WheelDeltaCarry = 0;

    bool bLeftIsPressed  = false;
    bool bRightIsPressed = false;
    bool bIsInWindow     = false;

    std::queue<FEvent> EventBuffer;
};
