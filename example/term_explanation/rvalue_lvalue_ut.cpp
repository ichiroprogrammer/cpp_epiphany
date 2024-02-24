#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace Rvalue {

void f()
// @@@ sample begin 0:0

{
    // sを初期化するためにstd::string{}により生成されるオブジェクトはprvalue
    // sはlvalue

    auto s = std::string{};

#if 0
    // 下記はコンパイルエラー

    auto* sp = &std::string{};
    // 下記はg++のエラーメッセージ
    // programming_convention_type.cpp|709 col 29| error: taking address of rvalue [-fpermissive]
    // ||   709 |     auto* sp = &std::string{};
#else
    // 下記のようにすればアドレスを取得できるが、このようなことはすべきではない。
    auto&& rvalue_ref = std::string{};
    auto   sp         = &rvalue_ref;
#endif
    static_assert(std::is_same_v<std::string*, decltype(sp)>);
    IGNORE_UNUSED_VAR(s, sp);  // @@@ delete
}
// @@@ sample end

void g0()
{
    // @@@ sample begin 0:1

    auto str0 = std::string{};        // str0はlvalue
    auto str1 = std::string{"hehe"};  // str1もlvalue
    str0      = str1;                 // lvalueからの代入
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}

void g1()
{
    // @@@ sample begin 0:2

    auto str0 = std::string{};        // str0はlvalue
    str0      = std::string{"hehe"};  // rvalueからの代入
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}

void g2()
{
    // @@@ sample begin 0:3

    auto str0 = std::string{};        // str0はlvalue
    auto str1 = std::string{"hehe"};  // str1もlvalue
    str0      = std::move(str1);      // str1はこれ以降使われないとする
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}

// @@@ sample begin 0:4

#define IS_LVALUE(EXPR_) std::is_lvalue_reference_v<decltype((EXPR_))>
#define IS_XVALUE(EXPR_) std::is_rvalue_reference_v<decltype((EXPR_))>
#define IS_PRVALUE(EXPR_) !std::is_reference_v<decltype((EXPR_))>
#define IS_RVALUE(EXPR_) (IS_PRVALUE(EXPR_) || IS_XVALUE(EXPR_))

TEST(Expression, rvalue)
{
    auto str = std::string{};

    static_assert(IS_LVALUE(str), "EXPR_ must be lvalue");
    static_assert(!IS_RVALUE(str), "EXPR_ must NOT be rvalue");

    static_assert(IS_XVALUE(std::move(str)), "EXPR_ must be xvalue");
    static_assert(!IS_PRVALUE(std::move(str)), "EXPR_ must NOT be prvalue");

    static_assert(IS_PRVALUE(std::string{}), "EXPR_ must be prvalue");
    static_assert(IS_RVALUE(std::string{}), "EXPR_ must be rvalue");
    static_assert(!IS_LVALUE(std::string{}), "EXPR_ must NOT be lvalue");
}
// @@@ sample end
}  // namespace Rvalue
namespace RvalueLvalue {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_UNUSED_VAR;
// @@@ sample begin 1:0

class C {
public:
    explicit C(char const* str) : str_{str} {}

    // lvalue修飾なし、rvalue修飾なし
    std::string& GetString0() noexcept { return str_; }

    // lvalue修飾
    std::string const& GetString1() const& noexcept { return str_; }

    // rvalue修飾
    // *thisがrvalueの場合でのGetString1()の呼び出しは、この関数を呼び出すため、
    // class内部のハンドルを返してはならない。
    // また、それによりstd::stringを生成するため、noexcept指定してはならない。
    std::string GetString1() const&& { return str_; }

    // lvalue修飾だが、const関数はrvalueからでも呼び出せる。
    // rvalueに対しての呼び出しを禁止したい場合には、GetString4のようにする。
    std::string const& GetString2() const& noexcept { return str_; }

    // lvalue修飾
    // 非constなのでrvalueからは呼び出せない。
    std::string const& GetString3() & noexcept { return str_; }

    // lvalue修飾
    std::string const& GetString4() const& noexcept { return str_; }

    // rvalue修飾
    // rvalueからこの関数を呼び出されるとrvalueオブジェクトの内部ハンドルを返してしまい、
    // 危険なので=deleteすべき。
    std::string const& GetString4() const&& = delete;

private:
    std::string str_;
};
// @@@ sample end

void f()
{
    // @@@ sample begin 1:1

    auto        c    = C{"c0"};
    auto const& s0_0 = c.GetString0();        // OK cが解放されるまでs0_0は有効
    auto        s0_1 = C{"c1"}.GetString0();  // NG 危険なコード
    // s0_1が指すオブジェクトは、次の行で無効になる

    auto const& s1_0 = c.GetString1();        // OK GetString1()&が呼び出される
    auto const& s1_1 = C{"c1"}.GetString1();  // OK GetString1()&&が呼び出される
    // s1_0が指すrvalueはs1_0がスコープアウトするまで有効

    auto const& s2_0 = c.GetString2();        // OK GetString2()&が呼び出される
    auto const& s2_1 = C{"c1"}.GetString2();  // NG const関数はlvalue修飾しても呼び出し可能
    // s2_1が指すオブジェクトは、次の行で無効になる

    auto const& s3_0 = c.GetString3();  // OK GetString3()&が呼び出される
    // auto const& s3_1 = C{"c1"}.GetString3();  // 危険なのでコンパイルさせない

    auto const& s4_0 = c.GetString4();  // OK GetString4()&が呼び出される
    // auto const& s4_1 = C{"c1"}.GetString4();  // 危険なのでコンパイルさせない
    // @@@ sample end
}
SUPPRESS_WARN_END;
}  // namespace RvalueLvalue
