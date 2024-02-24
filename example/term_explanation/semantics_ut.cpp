#include <memory>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

TEST(Semantics, equality)
{
    {
        // @@@ sample begin 0:0

        auto  a = 0;
        auto& b = a;

        ASSERT_TRUE(a == b);
        ASSERT_TRUE(&a == &b);  // aとbは同一
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:1

        auto a = 0;
        auto b = 0;

        ASSERT_TRUE(a == b);
        ASSERT_FALSE(&a == &b);  // aとbは同一ではない
        // @@@ sample end
    }
}

// @@@ sample begin 1:0

class A {
public:
    explicit A(int num, char const* name) noexcept : num_{num}, name_{name} {}

    int         GetNum() const noexcept { return num_; }
    char const* GetName() const noexcept { return name_; }

private:
    int const   num_;
    char const* name_;
};
// @@@ sample end

namespace BadEquality {

// @@@ sample begin 1:1

inline bool operator==(A const& lhs, A const& rhs) noexcept
{
    return lhs.GetNum() == rhs.GetNum() && lhs.GetName() == rhs.GetName();
}
// @@@ sample end

TEST(Semantics, bad_equality_A)
{
    {
        // @@@ sample begin 1:2

        auto a0 = A{0, "a"};
        auto a1 = A{0, "a"};

        ASSERT_TRUE(a0 == a1);
        // @@@ sample end
    }

    {
        // @@@ sample begin 1:3

        char a0_name[] = "a";
        auto a0        = A{0, a0_name};

        char a1_name[] = "a";
        auto a1        = A{0, a1_name};

#if 0   // @@@ delete
        ASSERT_TRUE(a0 == a1);  // テストが失敗する
#endif  // @@@ delete
        // @@@ sample end
        ASSERT_FALSE(a0 == a1);
    }
}
}  // namespace BadEquality

namespace GoodEquality {

// @@@ sample begin 1:4

inline bool operator==(A const& lhs, A const& rhs) noexcept
{
    return lhs.GetNum() == rhs.GetNum()
           && std::string_view{lhs.GetName()} == std::string_view{rhs.GetName()};
}
// @@@ sample end

TEST(Semantics, good_equality_A)
{
    char a0_name[] = "a";
    auto a0        = A{0, a0_name};

    char a1_name[] = "a";
    auto a1        = A{0, a1_name};

    ASSERT_TRUE(a0 == a1);
}
}  // namespace GoodEquality

namespace BadEquality {

// @@@ sample begin 2:0

class Base {
public:
    explicit Base(int b) noexcept : b_{b} {}
    virtual ~Base() = default;
    int GetB() const noexcept { return b_; }

private:
    int b_;
};

inline bool operator==(Base const& lhs, Base const& rhs) noexcept
{
    return lhs.GetB() == rhs.GetB();
}
// @@@ sample end

TEST(Semantics, bad_equality_B)
{
    // @@@ sample begin 2:1

    auto b0 = Base{0};
    auto b1 = Base{0};
    auto b2 = Base{1};

    ASSERT_TRUE(b0 == b0);
    ASSERT_TRUE(b0 == b1);
    ASSERT_FALSE(b0 == b2);
    // @@@ sample end
}

// @@@ sample begin 3:0

class Derived : public Base {
public:
    explicit Derived(int d) noexcept : Base{0}, d_{d} {}
    int GetD() const noexcept { return d_; }

private:
    int d_;
};
// @@@ sample end

TEST(Semantics, bad_equality_D)
{
    // @@@ sample begin 3:1

    {
        auto b = Base{0};
        auto d = Derived{1};

        ASSERT_TRUE(b == d);  // NG bとdは明らかに等価でない
    }
    {
        auto d0 = Derived{0};
        auto d1 = Derived{1};

        ASSERT_TRUE(d0 == d1);  // NG d0とd1は明らかに等価ではない
    }
    // @@@ sample end
}

// @@@ sample begin 4:0

bool operator==(Derived const& lhs, Derived const& rhs) noexcept
{
    return lhs.GetB() == rhs.GetB() && lhs.GetD() == rhs.GetD();
}
// @@@ sample end

TEST(Semantics, bad_equality_D2)
{
    // @@@ sample begin 4:1

    auto d0 = Derived{0};
    auto d1 = Derived{1};

    ASSERT_FALSE(d0 == d1);  // OK operator==(Derived const&, Derived const&)の効果で正しい判定

    Base& d0_b_ref = d0;

    ASSERT_TRUE(d0_b_ref == d1);  // NG d0_b_refの実態はd0なのでd1と等価でない
    // @@@ sample end
}
}  // namespace BadEquality

namespace GoodEquality {

// @@@ sample begin 5:0

class Base {
public:
    explicit Base(int b) noexcept : b_{b} {}
    virtual ~Base() = default;
    int GetB() const noexcept { return b_; }

protected:
    virtual bool is_equal(Base const& rhs) const noexcept { return b_ == rhs.b_; }

private:
    int b_;

    friend inline bool operator==(Base const& lhs, Base const& rhs) noexcept
    {
        if (typeid(lhs) != typeid(rhs)) {
            return false;
        }

        return lhs.is_equal(rhs);
    }
};

class Derived : public Base {
public:
    explicit Derived(int d) : Base{0}, d_{d} {}
    int GetD() const noexcept { return d_; }

protected:
    virtual bool is_equal(Base const& rhs) const noexcept
    {
        // operator==によりrhsの型はDerivedであるため、下記のキャストは安全
        auto const& rhs_d = static_cast<Derived const&>(rhs);

        return Base::is_equal(rhs) && d_ == rhs_d.d_;
    }

private:
    int d_;
};
// @@@ sample end

TEST(Semantics, good_equality_D)
{
    {
        auto d0 = Derived{0};
        auto d1 = Derived{1};

        ASSERT_FALSE(d0 == d1);  // OK operator==(Derived const&, Derived const&)の効果で正しい判定

        Base& d0_b_ref = d0;

        ASSERT_FALSE(d0_b_ref == d1);  // OK d0_b_refの実態はd0なのでd1と等価でない
    }
    {
        auto  b    = Base{0};
        auto  d0   = Derived{1};
        auto  d1   = Derived{1};
        Base& b_d1 = d1;

        ASSERT_FALSE(b == d0);
        ASSERT_TRUE(d0 == d0);
        ASSERT_TRUE(d0 == d1);
        ASSERT_TRUE(d0 == b_d1);
    }
}
// @@@ sample begin 5:1

class DerivedDerived : public Derived {
public:
    explicit DerivedDerived(int dd) noexcept : Derived{0}, dd_{dd} {}
    int GetDD() const noexcept { return dd_; }

protected:
    virtual bool is_equal(Base const& rhs) const noexcept
    {
        // operator==によりrhsの型はDerivedDerivedであるため、下記のキャストは安全
        auto const& rhs_d = static_cast<DerivedDerived const&>(rhs);

        return Derived::is_equal(rhs) && dd_ == rhs_d.dd_;
    }

private:
    int dd_;
};
// @@@ sample end

TEST(Semantics, good_equality_DD)
{
    auto  b1        = Base{1};
    auto  d1        = Derived{1};
    auto  dd0       = DerivedDerived{0};
    auto  dd1       = DerivedDerived{1};
    auto  dd1_2     = DerivedDerived{1};
    Base& dd1_b_ref = dd1;

    ASSERT_TRUE(b1 == b1);
    ASSERT_TRUE(d1 == d1);
    ASSERT_TRUE(dd0 == dd0);

    ASSERT_FALSE(b1 == d1);
    ASSERT_FALSE(b1 == dd0);
    ASSERT_FALSE(b1 == dd1);

    ASSERT_FALSE(d1 == b1);
    ASSERT_FALSE(d1 == dd0);
    ASSERT_FALSE(d1 == dd1);

    ASSERT_FALSE(dd0 == dd1);
    ASSERT_TRUE(dd1 == dd1_2);
    ASSERT_TRUE(dd1_b_ref == dd1_2);
}
}  // namespace GoodEquality

TEST(Semantics, string)
{
    // @@@ sample begin 6:0

    auto abc = std::string{"abc"};

    ASSERT_TRUE("abc" == abc);
    ASSERT_TRUE(abc == "abc");
    // @@@ sample end
}

namespace GoodEquality {

TEST(Semantics, copy_semantics)
{
    {
        // @@@ sample begin 7:0

        auto c_str = "string";
        auto str   = std::string{};

        str = c_str;
        ASSERT_TRUE(c_str == str);      // = 後には == が成立している
        ASSERT_STREQ("string", c_str);  // c_strの値は変わっていない
        // @@@ sample end
    }
    {
        SUPPRESS_WARN_BEGIN;
        SUPPRESS_WARN_DEPRECATED_DECL;
        // @@@ sample begin 7:1

        std::auto_ptr<std::string> str0{new std::string{"string"}};
        std::auto_ptr<std::string> str0_pre{new std::string{"string"}};

        ASSERT_TRUE(*str0 == *str0_pre);  // 前提は成立

        std::auto_ptr<std::string> str1;

        str1 = str0;

        // ASSERT_TRUE(*str0 == *str0_pre);  // これをするとクラッシュする
        ASSERT_TRUE(str0.get() == nullptr);  // str0の値がoperator ==で変わってしまった

        ASSERT_TRUE(*str1 == *str0_pre);  // これは成立

        // @@@ sample end
        SUPPRESS_WARN_END;
    }
    {
        // @@@ sample begin 7:2

        auto b = Base{1};
        auto d = Derived{1};

        b = d;  // スライシングが起こる

        ASSERT_FALSE(b == d);  // copyセマンティクスを満たしていない
        // @@@ sample end
    }
}
}  // namespace GoodEquality
// 本来Baseのデストラクタはvirtualでなければならないが、オブジェクトレイアウトを単純にするために
// 非virtualにする。

// @@@ sample begin 8:0

class NotRecommended {
public:
    NotRecommended(char const* name) : name_{name} {}
    std::string const& Name() const noexcept { return name_; }

    NotRecommended& operator=(NotRecommended&& rhs)  // move代入、非no-fail保証
    {
        name_ = rhs.name_;  // rhs.name_からname_へのcopy代入。パフォーマンス問題になるかも。
        return *this;
    }

private:
    std::string name_;
};

bool operator==(NotRecommended const& lhs, NotRecommended const& rhs) noexcept
{
    return lhs.Name() == rhs.Name();
}

TEST(Semantics, move1)
{
    auto a = NotRecommended{"a"};
    auto b = NotRecommended{"a"};

    ASSERT_EQ("a", a.Name());
    ASSERT_TRUE(a == b);

    auto c = NotRecommended{"c"};
    ASSERT_EQ("c", c.Name());

    c = std::move(a);
    ASSERT_TRUE(b == c);  // 一応、moveセマンティクスは守っているが・・・
}

// @@@ sample end
// @@@ sample begin 8:1

class Recommended {
public:
    Recommended(char const* name) : name_{name} {}
    std::string const& Name() const noexcept { return name_; }

    Recommended& operator=(Recommended&& rhs) noexcept  // move代入、no-fail保証
    {
        name_ = std::move(rhs.name_);  // rhs.name_からname_へのmove代入
        return *this;
    }

private:
    std::string name_;
};

bool operator==(Recommended const& lhs, Recommended const& rhs) noexcept
{
    return lhs.Name() == rhs.Name();
}

TEST(Semantics, move2)
{
    auto a = Recommended{"a"};
    auto b = Recommended{"a"};

    ASSERT_EQ("a", a.Name());
    ASSERT_TRUE(a == b);

    auto c = Recommended{"c"};
    ASSERT_EQ("c", c.Name());

    c = std::move(a);     // これ以降aは使ってはならない
    ASSERT_TRUE(b == c);  // moveセマンティクスを正しく守っている
}

// @@@ sample end

}  // namespace
