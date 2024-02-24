#include <cstdint>

#include "gtest_wrapper.h"

namespace {
namespace AntiPatternSample {

// @@@ sample begin 0:0

class A {  // アンチパターン
public:
    int32_t a_{0};
};

void f(A& a) noexcept
{
    a.a_ = 3;

    // Do something
    // @@@ ignore begin
    // @@@ ignore end
}
// @@@ sample end
}  // namespace AntiPatternSample

namespace PatternSample {

// @@@ sample begin 1:0

class A {  // Accessorの実装例
public:
    void SetA(int32_t a) noexcept  // setter
    {
        a_ = a;
    }

    int32_t GetA() const noexcept  // getter
    {
        return a_;
    }

private:
    int32_t a_{0};
    // @@@ ignore begin
    A& operator=(A const&) = delete;
    // @@@ ignore end
};

void f(A& a) noexcept
{
    a.SetA(3);

    // Do something
    // @@@ ignore begin
    // @@@ ignore end
}
// @@@ sample end
}  // namespace PatternSample

namespace AntiPatternSample2 {

// @@@ sample begin 2:0

class A {  // Accessorを使用して細かすぎる制御をしてしまうアンチパターン
public:
    void SetA(int32_t a) noexcept  // setter
    {
        a_ = a;
    }

    int32_t GetA() const noexcept  // getter
    {
        return a_;
    }

    void Change(bool is_changed) noexcept  // setter
    {
        is_changed_ = is_changed;
    }

    bool IsChanged() const noexcept  // getter
    {
        return is_changed_;
    }

    void DoSomething() noexcept  // is_changed_がtrueの時に、呼び出してほしい
    {
        // Do something
        // @@@ ignore begin
        // @@@ ignore end
    }
    // @@@ ignore begin

private:
    int32_t a_{0};
    bool    is_changed_{false};

    A& operator=(A const&) = delete;
    // @@@ ignore end
};

void f(A& a) noexcept
{
    if (a.GetA() != 3) {
        a.SetA(3);
        a.Change(true);
    }

    // @@@ ignore begin
    // @@@ ignore end
}

void g(A& a) noexcept
{
    if (!a.IsChanged()) {
        return;
    }

    a.Change(false);
    a.DoSomething();  // a.IsChanged()がtrueの時に実行する。

    // @@@ ignore begin
    // @@@ ignore end
}
// @@@ sample end
}  // namespace AntiPatternSample2

namespace PatternSample2 {

// @@@ sample begin 3:0

class A {  // 上記アンチパターンからChange()とIsChanged()を削除し、状態の隠蔽レベルを強化
public:
    void SetA(int32_t a) noexcept  // setter
    {
        if (a_ == a) {
            return;
        }

        a_          = a;
        is_changed_ = true;
    }

    void DoSomething() noexcept
    {
        if (!is_changed_) {
            return;
        }

        // Do something
        // @@@ ignore begin
        // @@@ ignore end

        is_changed_ = false;  // 状態変更の取り消し
    }
    // @@@ ignore begin

private:
    int32_t a_{0};
    bool    is_changed_{false};

    A& operator=(A const&) = delete;
    // @@@ ignore end
};

void f(A& a) noexcept
{
    a.SetA(3);

    // @@@ ignore begin
    // @@@ ignore end
}

void g(A& a) noexcept
{
    a.DoSomething();  // DoSomethingは無条件で呼び出す。
                      // 実際に何かをするかどうかは、オブジェクトaが決める。
    // @@@ ignore begin
    // @@@ ignore end
}
// @@@ sample end
}  // namespace PatternSample2

TEST(Accessor, just_call)
{
    {
        using namespace AntiPatternSample;
        auto a = A{};
        f(a);
    }
    {
        using namespace PatternSample;
        auto a = A{};
        f(a);
    }
    {
        using namespace AntiPatternSample2;
        auto a = A{};
        f(a);
        g(a);
    }
    {
        using namespace PatternSample2;
        auto a = A{};
        f(a);
        g(a);
    }
}
}  // namespace
