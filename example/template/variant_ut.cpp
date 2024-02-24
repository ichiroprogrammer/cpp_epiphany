#include <cassert>
#include <sstream>
#include <variant>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"

namespace {

TEST(Template, variant_usage)
{
    // @@@ sample begin 0:0

    auto v = std::variant<int, std::string, double>{};  // 3つの型を切り替える

    // std::get<N>()の戻り値型は、下記の通りで、
    // N == 0, 1, 2 は、それぞれint, std::string, doubleに対応
    static_assert(std::is_same_v<decltype(std::get<0>(v)), int&>);
    static_assert(std::is_same_v<decltype(std::get<1>(v)), std::string&>);
    static_assert(std::is_same_v<decltype(std::get<2>(v)), double&>);

    v = int{3};  // int型の3を代入

    ASSERT_EQ(v.index(), 0);                                // intを保持
    ASSERT_EQ(std::get<0>(v), 3);                           // intなので問題なくアクセス
    ASSERT_THROW(std::get<1>(v), std::bad_variant_access);  // std::stringではないのでエクセプション
    ASSERT_THROW(std::get<2>(v), std::bad_variant_access);  // doubleではないのでエクセプション

    v = std::string{"str"};  // std::stringオブジェクトを代入

    ASSERT_EQ(v.index(), 1);                                // std::stringを保持
    ASSERT_THROW(std::get<0>(v), std::bad_variant_access);  // intではないのでエクセプション
    ASSERT_EQ(std::get<1>(v), std::string{"str"});  // std::stringなので問題なくアクセス
    ASSERT_THROW(std::get<2>(v), std::bad_variant_access);  // doubleではないのでエクセプション
    // @@@ sample end
    // @@@ sample begin 0:1

    auto oss = std::ostringstream{};

    // type_valueはvが保持する型をその値を文字列で返す
    auto type_value = [&oss](auto const& v) {  // ジェネリックラムダでなくても実装可能
        if (v.index() == 0) {
            auto a  = std::get<0>(v);
            using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
            oss << Nstd::Type2Str<T>() << " : " << a;  // Nstd::Type2Str<T>はTの型を文字列にする
        }
        else if (v.index() == 1) {
            auto a  = std::get<1>(v);
            using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
            oss << Nstd::Type2Str<T>() << " : " << a;  // Nstd::Type2Str<T>はTの型を文字列にする
        }
        else if (v.index() == 2) {
            auto a  = std::get<2>(v);
            using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
            oss << Nstd::Type2Str<T>() << " : " << a;  // Nstd::Type2Str<T>はTの型を文字列にする
        }
        else {
            assert(false);  // ここには来ないはず
        }
    };

    v = 3;  // int型の3を代入
    type_value(v);
    ASSERT_EQ("int : 3", oss.str());
    oss = std::ostringstream{};  // ossのリセット

    v = std::string{"str"};  // std::stringオブジェクトを代入
    type_value(v);
    ASSERT_EQ("std::string : str", oss.str());
    oss = std::ostringstream{};  // ossのリセット

    v = 1.1;  // double型の1.1を代入
    type_value(v);
    ASSERT_EQ("double : 1.1", oss.str());
    // @@@ sample end
}

// @@@ sample begin 1:0

template <typename VARIANT, typename F, size_t INDEX = 0>
void org_visit(const F& f, const VARIANT& v)
{
    constexpr auto n = std::variant_size_v<VARIANT>;

    if constexpr (INDEX < n) {
        if (v.index() == INDEX) {  // 保持する型が見つかった
            f(std::get<INDEX>(v));
            return;
        }
        else {  // 保持する型が見つかるまでリカーシブ
            org_visit<VARIANT, F, INDEX + 1>(f, v);
        }
    }
    else {
        assert(false);  // ここには来ないはず
    }
}
// @@@ sample end

TEST(Template, variant)
{
    // @@@ sample begin 1:1

    auto oss = std::ostringstream{};

    // 文字列を返すためのジェネリックラムダ
    auto type_value = [&oss](auto const& a) {
        using T = std::remove_const_t<std::remove_reference_t<decltype(a)>>;
        oss << Nstd::Type2Str<T>() << " : " << a;
    };
    // @@@ sample end
    // @@@ sample begin 1:2
    {
        auto v = std::variant<int, std::string, double>{};  // 3つの型を切り替える

        v = 3;
        org_visit(type_value, v);
        ASSERT_EQ("int : 3", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        // @@@ ignore begin
        v = std::string{"str"};
        org_visit(type_value, v);
        ASSERT_EQ("std::string : str", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        v = 1.1;
        org_visit(type_value, v);
        ASSERT_EQ("double : 1.1", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        v = 3;
        std::visit(type_value, v);
        ASSERT_EQ("int : 3", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        v = std::string{"str"};
        std::visit(type_value, v);
        ASSERT_EQ("std::string : str", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        v = 1.1;
        std::visit(type_value, v);
        ASSERT_EQ("double : 1.1", oss.str());
        oss = std::ostringstream{};  // ossのリセット
        // @@@ ignore end
    }
    {
        auto v = std::variant<char, int, std::string, double>{};  // 4つの型を切り替える

        v = 3;
        org_visit(type_value, v);
        ASSERT_EQ("int : 3", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        v = 'c';
        org_visit(type_value, v);
        ASSERT_EQ("char : c", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        // @@@ ignore begin
        v = std::string{"str"};
        org_visit(type_value, v);
        ASSERT_EQ("std::string : str", oss.str());
        oss = std::ostringstream{};  // ossのリセット

        v = 1.1;
        org_visit(type_value, v);
        ASSERT_EQ("double : 1.1", oss.str());
        oss = std::ostringstream{};  // ossのリセット
        // @@@ ignore end
    }
    // @@@ sample end
}

// @@@ sample begin 2:0

class A {
public:
    char f() const noexcept { return 'A'; }
};

class B {
public:
    char f() const noexcept { return 'B'; }
};

class C {
public:
    char f() const noexcept { return 'C'; }
};
// @@@ sample end

TEST(Template, variant_abc)
{
    // @@@ sample begin 2:1

    char ret{};
    auto call_f = [&ret](auto const& a) { ret = a.f(); };

    auto v = std::variant<A, B, C>{};

    org_visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('A', ret);

    v = B{};
    org_visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('B', ret);

    v = C{};
    org_visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('C', ret);
    // @@@ sample end
    // @@@ sample begin 2:2

    v = A{};
    std::visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('A', ret);

    v = B{};
    std::visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('B', ret);

    v = C{};
    std::visit(call_f, v);  // 疑似的なポリモーフィズム
    ASSERT_EQ('C', ret);
    // @@@ sample end
}
}  // namespace
