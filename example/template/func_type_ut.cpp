#include <functional>
#include <memory>

#include "gtest_wrapper.h"

#include "scoped_guard.h"

namespace {

// @@@ sample begin 0:0

// デストラクタが呼び出された時に、外部から渡されたフラグをtrueにする
struct A {
    explicit A(bool& destructor_called) noexcept : destructor_called{destructor_called} {}
    ~A() { destructor_called = true; };

    bool& destructor_called;
};
// @@@ sample end
// @@@ sample begin 0:1

void delete_func(A* a) noexcept { delete a; }
// @@@ sample end

TEST(Template, function)
{
    // @@@ sample begin 1:0

    {  // 第2パラメータに何も指定しない
        auto is_called = false;
        {
            auto ua = std::unique_ptr<A>{new A{is_called}};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
    // @@@ sample end
    // @@@ sample begin 1:1

    {  // 第2パラメータに関数ポインタを与える
        auto is_called = false;
        {
            auto ua = std::unique_ptr<A, void (*)(A*)>{new A{is_called}, &delete_func};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
    // @@@ sample end
    // @@@ sample begin 1:2

    {  // 第2パラメータにラムダを与える
        auto is_called = false;
        {
            auto delete_lambda = [](A* a) noexcept { delete a; };

            // ラムダ式の型はインスタンス毎に異なるため、
            // ラムダ式の型を取得するためには下記のようにdecltypeを使う必要がある
            auto ua = std::unique_ptr<A, decltype(delete_lambda)>{new A{is_called}, delete_lambda};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
    // @@@ sample end
    // @@@ sample begin 1:3

    {  // 第2パラメータにstd::function型オブジェクトを与える
        auto is_called = false;
        {
            auto delete_obj = std::function<void(A*)>{[](A* a) noexcept { delete a; }};
            auto ua = std::unique_ptr<A, std::function<void(A*)>&>{new A{is_called}, delete_obj};
            ASSERT_FALSE(is_called);  // uaのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // uaのデストラクタは呼ばれた
    }
    // @@@ sample end
}

namespace SG_Template {
// for test
// @@@ sample begin 2:0

bool is_caleded_in_static{false};
void caleded_by_destructor() noexcept { is_caleded_in_static = true; }
// @@@ sample end

TEST(Template, scoped_guard_0)
{
    // @@@ sample begin 2:1

    {  // Fに関数ポインタを与える
        is_caleded_in_static = false;
        {
            auto sg = ScopedGuard{&caleded_by_destructor};
            ASSERT_FALSE(is_caleded_in_static);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_caleded_in_static);  // sgのデストラクタは呼ばれた
    }
    // @@@ sample end
}

TEST(Template, scoped_guard_1)
{
    // @@@ sample begin 2:2

    {  // Fにラムダ式を与える
        auto is_called = false;
        {
            auto gs = ScopedGuard{[&is_called]() noexcept { is_called = true; }};
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    {  // Fにstd::function型オブジェクトを与える
        auto is_called = false;
        {
            auto f  = std::function<void(void)>{[&is_called]() noexcept { is_called = true; }};
            auto gs = ScopedGuard{std::move(f)};  // sgのデストラクタは呼ばれていない
            ASSERT_FALSE(is_called);              // sgのデストラクタは呼ばれた
        }
        ASSERT_TRUE(is_called);
    }
    // @@@ sample end
}

// @@@ sample begin 3:0

struct TestFunctor {
    explicit TestFunctor(bool& is_called) : is_called_{is_called} {}
    void  operator()() noexcept { is_called_ = true; }
    bool& is_called_;
};
// @@@ sample end

TEST(Template, scoped_guard_2)
{
    // @@@ sample begin 3:1

    {  // Fに関数型オブジェクトを与える
        auto is_called = false;
        auto tf        = TestFunctor{is_called};
        {
            auto sg = ScopedGuard{std::move(tf)};  // C++17以降の記法
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    // @@@ sample end
    // @@@ sample begin 3:2
    {  // Fに関数型オブジェクトを与える
        auto is_called = false;
        auto tf        = TestFunctor{is_called};
        {
            auto sg = ScopedGuard<TestFunctor>{std::move(tf)};  // C++14以前の記法
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    // @@@ sample end
}

// @@@ sample begin 4:0

template <typename F>
ScopedGuard<F> MakeScopedGuard(F&& f) noexcept
{
    return ScopedGuard<F>(std::move(f));
}
// @@@ sample end

TEST(Template, scoped_guard2)
{
    // @@@ sample begin 4:1

    {  // Fに関数ポインタを与える
        is_caleded_in_static = false;
        {
            auto sg = MakeScopedGuard(&caleded_by_destructor);
            ASSERT_FALSE(is_caleded_in_static);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_caleded_in_static);  // sgのデストラクタは呼ばれた
    }
    {  // Fにラムダ式を与える
        auto is_called = false;
        {
            auto sg = MakeScopedGuard([&is_called]() noexcept { is_called = true; });
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    {  // Fにstd::function型オブジェクトを与える
        auto is_called = false;
        {
            auto f  = std::function<void(void)>{[&is_called]() noexcept { is_called = true; }};
            auto sg = MakeScopedGuard(std::move(f));
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    {  // Fに関数型オブジェクトを与える
        auto is_called = false;
        auto tf        = TestFunctor{is_called};
        {
            auto sg = MakeScopedGuard(std::ref(tf));  // std::refが必要
            ASSERT_FALSE(is_called);  // sgのデストラクタは呼ばれていない
        }
        ASSERT_TRUE(is_called);  // sgのデストラクタは呼ばれた
    }
    // @@@ sample end
}
}  // namespace SG_Template
}  // namespace
