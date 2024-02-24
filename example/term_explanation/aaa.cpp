#include <mutex>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

TEST(AAA, not_commit_specific_type)
{
    // @@@ sample begin 0:0

    auto i  = 1;
    auto ui = 1U;
    auto d  = 1.0;
    auto s  = "str";
    auto v  = {0, 1, 2};

    for (auto i : v) {
        // 何らかの処理
        IGNORE_UNUSED_VAR(i);  // @@@ delete
    }

    auto add = [](auto lhs, auto rhs) {  // -> return_typeのような記述は不要
        return lhs + rhs;                // addの型もautoで良い
    };

    // 上記変数の型の確認
    static_assert(std::is_same_v<decltype(i), int>);
    static_assert(std::is_same_v<decltype(ui), unsigned int>);
    static_assert(std::is_same_v<decltype(d), double>);
    static_assert(std::is_same_v<decltype(s), char const*>);
    static_assert(std::is_same_v<decltype(v), std::initializer_list<int>>);

    char s2[] = "str";  // 配列の宣言には、AAAは使えない
    static_assert(std::is_same_v<decltype(s2), char[4]>);

    int* p0 = nullptr;  // 初期値がnullptrであるポインタの初期化には、AAAは使うべきではない
    auto p1 = static_cast<int*>(nullptr);  // NG
    auto p2 = p0;                          // OK
    auto p3 = nullptr;                     // NG 通常、想定通りにならない
    static_assert(std::is_same_v<decltype(p3), std::nullptr_t>);
    // @@@ sample end

    IGNORE_UNUSED_VAR(i, ui, d, s, v, s2, p0, p1, p2, p3);
    ASSERT_EQ(2, add(1, 1));
}

TEST(AAA, commit_specific_type)
{
    // @@@ sample begin 1:0

    auto b  = new char[10]{0};
    auto v  = std::vector<int>{0, 1, 2};
    auto s  = std::string{"str"};
    auto sv = std::string_view{"str"};

    static_assert(std::is_same_v<decltype(b), char*>);
    static_assert(std::is_same_v<decltype(v), std::vector<int>>);
    static_assert(std::is_same_v<decltype(s), std::string>);
    static_assert(std::is_same_v<decltype(sv), std::string_view>);

    // 大量のstd::stringオブジェクトを定義する場合
    using std::literals::string_literals::operator""s;

    auto s_0 = "222"s;  // OK
    // ...
    auto s_N = "222"s;  // OK

    static_assert(std::is_same_v<decltype(s_0), std::string>);
    static_assert(std::is_same_v<decltype(s_N), std::string>);

    // 大量のstd::string_viewオブジェクトを定義する場合
    using std::literals::string_view_literals::operator""sv;

    auto sv_0 = "222"sv;  // OK
    // ...
    auto sv_N = "222"sv;  // OK

    static_assert(std::is_same_v<decltype(sv_0), std::string_view>);
    static_assert(std::is_same_v<decltype(sv_N), std::string_view>);

    std::mutex mtx;  // std::mutexはmove出来ないので、AAAスタイル不可
    auto       lock = std::lock_guard{mtx};

    static_assert(std::is_same_v<decltype(lock), std::lock_guard<std::mutex>>);
    // @@@ sample end

    delete[] b;
}

TEST(AAA, func_simple)
{
    // @@@ sample begin 2:0

    auto v = std::vector<int>{0, 1, 2};

    // AAAを使わない例
    std::vector<int>::size_type t0{v.size()};      // 正確に書くとこうなる
    std::vector<int>::iterator  itr0 = v.begin();  // 正確に書くとこうなる

    std::unique_ptr<int> p0 = std::make_unique<int>(3);

    // 上記をAAAにした例
    auto t1   = v.size();   // size()の戻りは算術型であると推測できる
    auto itr1 = v.begin();  // begin()の戻りはイテレータであると推測できる

    auto p1 = std::make_unique<int>(3);  // make_uniqueの戻りはstd::unique_ptrであると推測できる
    // @@@ sample end

    IGNORE_UNUSED_VAR(t0, t1, itr0, itr1, p0, p1);
}

std::map<std::string, int> gen_map()
{
    return std::map<std::string, int>{std::make_pair(std::string{"123"}, 123)};
}

TEST(AAA, func_complex)
{
    // @@@ sample begin 2:1

    extern std::map<std::string, int> gen_map();

    // 上記のような複雑な型を戻す関数の場合、AAAを使うと可読性が落ちる
    auto map0 = gen_map();

    for (auto [str, i] : gen_map()) {
        // 何らかの処理
    }

    // 上記のような複雑な型を戻す関数の場合、AAAを使うと可読性が落ちるため、AAAにしない
    std::map<std::string, int> map1 = gen_map();  // 型がコメントとして役に立つ

    for (std::pair<std::string, int> str_i : gen_map()) {
        // 何らかの処理
    }

    // 型を明示したAAAスタイルでも良い
    auto map2 = std::map<std::string, int>{gen_map()};  // 型がコメントとして役に立つ
    // @@@ sample end

    IGNORE_UNUSED_VAR(map0, map1, map2);
}

int32_t func(int32_t) { return 0; }

// @@@ sample begin 3:0

template <typename F, typename T>
auto apply_0(F&& f, T value)
{
    return f(value);
}
// @@@ sample end
// @@@ sample begin 3:1

template <typename F, typename T>
auto apply_1(F&& f, T value) -> decltype(f(std::declval<T>()))  // autoを使用しているが、AAAではない
{
    auto cond  = false;
    auto param = value;

    // 複雑な処理

    if (cond) {
        return f(param);
    }
    else {
        return f(value);
    }
}
// @@@ sample end

TEST(AAA, inline_func)
{
    ASSERT_EQ(0, apply_0(func, 0));
    ASSERT_EQ(0, apply_1(func, 0));
}

TEST(AAA, func_dinger)
{
    // @@@ sample begin 4:0

    auto v = std::vector<int>{0, 1, 2};

    int t0 = v.size();  // 縮小型変換されるため、バグが発生する可能性がある
    // int t1{v.size()};   縮小型変換のため、コンパイルエラー
    auto t2 = v.size();  // t2は正確な型
    // @@@ sample end

    IGNORE_UNUSED_VAR(t0, t2);
}

TEST(AAA, type_deduction)
{
    // @@@ sample begin 5:0

    auto str0 = "str";
    static_assert(std::is_same_v<char const*, decltype(str0)>);  // str0はchar[4]ではない

    // char[]が必要ならば、AAAを使わずに下記のように書く
    char str1[] = "str";
    static_assert(std::is_same_v<char[4], decltype(str1)>);

    // &が必要になるパターン
    class X {
    public:
        explicit X(int32_t a) : a_{a} {}
        int32_t& Get() { return a_; }

    private:
        int32_t a_;
    };

    X x{3};

    auto a0 = x.Get();
    ASSERT_EQ(3, a0);

    a0 = 4;
    ASSERT_EQ(4, a0);
    ASSERT_EQ(3, x.Get());  // a0はリファレンスではないため、このような結果になる

    // X::a_のリファレンスが必要ならば、下記のように書く
    auto& a1 = x.Get();
    a1       = 4;
    ASSERT_EQ(4, a1);
    ASSERT_EQ(4, x.Get());  // a1はリファレンスであるため、このような結果になる

    // constが必要になるパターン
    class Y {
    public:
        std::string&       Name() { return name_; }
        std::string const& Name() const { return name_; }

    private:
        std::string name_{"str"};
    };

    auto const y = Y{};

    auto        name0 = y.Name();  // std::stringがコピーされる
    auto&       name1 = y.Name();  // name1はconstに見えない
    auto const& name2 = y.Name();  // このように書くべき

    static_assert(std::is_same_v<std::string, decltype(name0)>);
    static_assert(std::is_same_v<std::string const&, decltype(name1)>);
    static_assert(std::is_same_v<std::string const&, decltype(name2)>);

    // 範囲for文でのauto const&
    auto const v = std::vector<std::string>{"0", "1", "2"};

    for (auto s : v) {  // sはコピー生成される
        static_assert(std::is_same_v<std::string, decltype(s)>);
        IGNORE_UNUSED_VAR(s);  // @@@ delete
    }

    for (auto& s : v) {  // sはconstに見えない
        static_assert(std::is_same_v<std::string const&, decltype(s)>);
        IGNORE_UNUSED_VAR(s);  // @@@ delete
    }

    for (auto const& s : v) {  // このように書くべき
        static_assert(std::is_same_v<std::string const&, decltype(s)>);
        IGNORE_UNUSED_VAR(s);  // @@@ delete
    }
    // @@@ sample end

    IGNORE_UNUSED_VAR(str0, str1, name1, name2);
}

}  // namespace
