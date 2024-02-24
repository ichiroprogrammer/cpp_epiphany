#include "gtest_wrapper.h"

#include "lsp.h"

namespace {
namespace Abort {
#ifdef __CYGWIN__  // cygwinでは、ASSERT_DEATHを使わない
#define LSP_Opt DISABLED_LSP
#else  // __linux__
#define LSP_Opt LSP
#endif

// @@@ sample begin 0:0

void SetX(Rectangle& rect, int x) noexcept { rect.SetX(x); }

TEST(LSP_Opt, violation_abort)
{
    // Rectangleのテスト
    auto rect = Rectangle{0, 0};
    SetX(rect, 3);
    ASSERT_EQ(3, rect.GetX());

    // Squareのテスト
    auto square = Square{0};
    ASSERT_DEATH(SetX(square, 3), "");  // ここでRectangle::SetX()の中のassert()がfailする。
}
// @@@ sample end
}  // namespace Abort

namespace NotAbort {
// @@@ sample begin 1:0

void SetX(Rectangle& rect, int x) noexcept
{
    if (dynamic_cast<Square*>(&rect) != nullptr) {
        rect = Square(x);
    }
    else {
        // rectの型は、Rectangle
        rect.SetX(x);
    }
}

TEST(LSP, violation_not_abort)
{
    // Rectangleのテスト
    auto rect = Rectangle{0, 0};
    SetX(rect, 3);
    ASSERT_EQ(3, rect.GetX());

    // Squareのテスト
    auto square = Square{0};
    SetX(square, 3);  // assert()はfailしない。
    ASSERT_EQ(3, square.GetX());
}
// @@@ sample end
}  // namespace NotAbort

namespace NotAbort2 {
// @@@ sample begin 2:0

void SetX(Rectangle& rect, int x) noexcept
{
    auto y = rect.GetY();
    rect   = Rectangle(x, y);
}
// @@@ sample end

TEST(LSP, violation_not_abort2)
{
    // Rectangleのテスト
    auto rect = Rectangle{0, 0};
    SetX(rect, 3);
    ASSERT_EQ(3, rect.GetX());

    // Squareのテスト
    auto square = Square{0};
    SetX(square, 3);  // assert()はfailしない。
    ASSERT_EQ(3, square.GetX());
}
}  // namespace NotAbort2
}  // namespace
