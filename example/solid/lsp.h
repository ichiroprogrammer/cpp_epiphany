
#include <cassert>
#include <iostream>

// @@@ sample begin 0:0

/// @class Rectangle
/// @brief (0, 0) からの矩形を表す
class Rectangle {
public:
    explicit Rectangle(int x, int y) noexcept : x_{x}, y_{y} {}
    // @@@ ignore begin
    virtual ~Rectangle() = default;

    // @@@ ignore end
    void SetX(int x) noexcept
    {
        auto temp = y_;
        set_x(x);
        assert(temp == y_);  // 「set_xはy_に影響を与えない」が事後条件
    }
    // @@@ ignore begin

    void SetY(int y) noexcept
    {
        auto temp = x_;
        set_y(y);
        assert(temp == x_);  // 「set_yはx_に影響を与えない」が事後条件
    }

    int GetX() const noexcept { return x_; }

    int GetY() const noexcept { return y_; }
    // @@@ ignore end

protected:
    virtual void set_x(int x) noexcept { x_ = x; }

    // @@@ ignore begin
    virtual void set_y(int y) noexcept { y_ = y; }
    // @@@ ignore end

private:
    int x_;
    int y_;
};

/// @class Rectangle
/// @brief (0, 0) からの正方形を表す
class Square : public Rectangle {
public:
    explicit Square(int x) noexcept : Rectangle{x, x} {}
    // @@@ ignore begin
    virtual ~Square() = default;

    // @@@ ignore end
protected:
    virtual void set_x(int x) noexcept override
    {
        Rectangle::set_x(x);
        Rectangle::set_y(x);
    }

    virtual void set_y(int y) noexcept override { set_x(y); }
};
// @@@ sample end
