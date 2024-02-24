#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace InitializerListConstructorSample {
// @@@ sample begin 0:0

class E {
public:
    E() : str_{"default constructor"} {}

    // 初期化子リストコンストラクタ
    explicit E(std::initializer_list<uint32_t>) : str_{"initializer list constructor"} {}

    explicit E(uint32_t, uint32_t) : str_{"uint32_t uint32_t constructor"} {}

    std::string const& GetString() const { return str_; }

private:
    std::string const str_;
};

TEST(Constructor, initializer_list_constructor)
{
    E const e0;
    ASSERT_EQ("default constructor", e0.GetString());

    E const e1{};
    ASSERT_EQ("default constructor", e1.GetString());

    E const e2{3, 4};  // E::E(uint32_t, uint32_t)の呼び出しと区別が困難
    ASSERT_EQ("initializer list constructor", e2.GetString());

    E const e3(3, 4);  // E::E(std::initializer_list<uint32_t>)の呼び出しと区別が困難
    ASSERT_EQ("uint32_t uint32_t constructor", e3.GetString());
}
// @@@ sample end
}  // namespace InitializerListConstructorSample
namespace InheritingConstructor {
// @@@ sample begin 1:0

class Base {
public:
    explicit Base(int32_t b) noexcept : b_{b} {}
    virtual ~Base() = default;
    // @@@ ignore begin
    int32_t GetB() noexcept { return b_; }

private:
    int32_t b_;
    // @@@ ignore end
};

class Derived : public Base {
public:
    using Base::Base;  // 継承コンストラクタ
#if 0
    Derived(int32_t b) : Base{b} {}  // オールドスタイル
#endif
};

void f() noexcept
{
    Derived d{1};  // Derived::Derived(int32_t)が使える
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(d);
    // @@@ ignore end
}
// @@@ sample end
}  // namespace InheritingConstructor
namespace DelegatingConstructor {
// @@@ sample begin 2:0

class A {
public:
    explicit A(std::string str) : str_{std::move(str)}
    {
        // @@@ ignore begin
        // @@@ ignore end
    }

    explicit A(uint32_t num) : A{std::to_string(num)}  // 委譲コンストラクタ
    {
    }

private:
    std::string str_;
};
// @@@ sample end
}  // namespace DelegatingConstructor
