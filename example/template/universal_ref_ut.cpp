#include <type_traits>
#include <utility>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace GenVector {
// @@@ sample begin 0:0

std::vector<std::string> gen_vector(std::string const& s0, std::string const& s1)
{
    auto ret = std::vector<std::string>{};

    ret.push_back(s0);
    ret.push_back(s1);

    return ret;
}
// @@@ sample end

namespace {
TEST(Template, gen_vector)
{
    // @@@ sample begin 0:1

    auto a = std::string{"a"};
    auto b = std::string{"b"};

    auto v = gen_vector(a, std::move(b));

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("a", a);
    ASSERT_EQ("b", b);  // bはmoveされない
    // @@@ sample end
}
}  // namespace
}  // namespace GenVector

namespace GenVectorUniversalRef {
// @@@ sample begin 1:0

template <typename STR0, typename STR1>
std::vector<std::string> gen_vector(STR0&& s0, STR1&& s1)
{
    auto ret = std::vector<std::string>{};

    ret.push_back(s0);
    ret.push_back(s1);

    return ret;
}
// @@@ sample end

namespace {
TEST(Template, gen_vector_universal_ref)
{
    // @@@ sample begin 1:1

    auto a = std::string{"a"};
    auto b = std::string{"b"};

    auto v = gen_vector(a, std::move(b));

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("a", a);
    ASSERT_EQ("b", b);  // bはmoveされない
    // @@@ sample end
}

// @@@ sample begin 1:2

enum class ExpressionType { Lvalue, Rvalue };

template <typename T>
constexpr ExpressionType universal_ref2(T&& t)
{
    return std::is_lvalue_reference_v<decltype(t)> ? ExpressionType::Lvalue
                                                   : ExpressionType::Rvalue;
}

// std::pair<>::first  : universal_refの中のtのExpressionType
// std::pair<>::second : universal_ref2の中でtのExpressionType
template <typename T>
constexpr std::pair<ExpressionType, ExpressionType> universal_ref(T&& t)
{
    return std::make_pair(
        std::is_lvalue_reference_v<decltype(t)> ? ExpressionType::Lvalue : ExpressionType::Rvalue,
        universal_ref2(t));
}
// @@@ sample end

TEST(Template, universal_ref)
{
    // @@@ sample begin 1:3

    auto i = 0;

    constexpr auto p = universal_ref(i);

    static_assert(universal_ref2(i) == ExpressionType::Lvalue);
    static_assert(p.first == ExpressionType::Lvalue);
    static_assert(p.second == ExpressionType::Lvalue);

    constexpr auto pm = universal_ref(std::move(i));

    static_assert(universal_ref2(std::move(i)) == ExpressionType::Rvalue);
    static_assert(pm.first == ExpressionType::Rvalue);
    static_assert(pm.second == ExpressionType::Lvalue);

    constexpr auto pm2 = universal_ref(int{});

    static_assert(universal_ref2(int{}) == ExpressionType::Rvalue);
    static_assert(pm2.first == ExpressionType::Rvalue);
    static_assert(pm2.second == ExpressionType::Lvalue);
    // @@@ sample end

    ASSERT_EQ(0, i);
}
}  // namespace
}  // namespace GenVectorUniversalRef

namespace GenVectorUniversalRefWithForward {
// @@@ sample begin 2:0

template <typename STR0, typename STR1>
std::vector<std::string> gen_vector(STR0&& s0, STR1&& s1)
{
    auto ret = std::vector<std::string>{};

    ret.push_back(std::forward<STR0>(s0));
    ret.push_back(std::forward<STR1>(s1));

    return ret;
}
// @@@ sample end

namespace {
TEST(Template, gen_vector_universal_ref_with_forwad)
{
    {
        // @@@ sample begin 2:1

        auto a = std::string{"a"};
        auto b = std::string{"b"};

        auto v = gen_vector(a, std::move(b));

        ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
        ASSERT_EQ("a", a);
        ASSERT_EQ("", b);  // bはmoveされた
        // @@@ sample end
    }
    {
        // @@@ sample begin 2:2

        auto a = std::string{"a"};

        auto v = gen_vector(a, "b");

        ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
        ASSERT_EQ("a", a);
        // @@@ sample end
    }
}
}  // namespace
}  // namespace GenVectorUniversalRefWithForward

namespace GenVectorUniversalRefParameterPackWithForward {
// @@@ sample begin 3:0

void emplace_back(std::vector<std::string>&) noexcept {}

template <typename HEAD, typename... TAIL>
void emplace_back(std::vector<std::string>& strs, HEAD&& head, TAIL&&... tails)
{
    strs.emplace_back(std::forward<HEAD>(head));

    if constexpr (sizeof...(tails) != 0) {
        emplace_back(strs, std::forward<TAIL>(tails)...);
    }
}

template <typename... STR>
std::vector<std::string> gen_vector(STR&&... ss)
{
    auto ret = std::vector<std::string>{};

    emplace_back(ret, std::forward<STR>(ss)...);

    return ret;
}
// @@@ sample end

namespace {
TEST(Template, gen_vector_universal_ref_with_forwad)
{
    // @@@ sample begin 3:1

    auto a = std::string{"a"};
    auto b = std::string{"b"};

    auto v = gen_vector(a, std::move(b), "c");

    ASSERT_EQ((std::vector<std::string>{"a", "b", "c"}), v);
    ASSERT_EQ("a", a);
    ASSERT_EQ("", b);  // bはmoveされた
    // @@@ sample end
}
}  // namespace
}  // namespace GenVectorUniversalRefParameterPackWithForward

namespace GenVectorUniversalRef_Worst {

// @@@ sample begin 4:0

template <typename STR0, typename STR1>
std::vector<std::string> gen_vector(STR0&& s0, STR1&& s1)
{
    auto ret = std::vector<std::string>{};

    ret.push_back(std::move(s0));
    ret.push_back(std::move(s1));

    return ret;
}
// @@@ sample end

namespace {
TEST(Template, gen_vector_universal_ref_worst)
{
    // @@@ sample begin 4:1

    auto       a = std::string{"a"};
    auto const b = std::string{"b"};

    auto v = gen_vector(a, std::move(b));

    ASSERT_EQ((std::vector<std::string>{"a", "b"}), v);
    ASSERT_EQ("", a);   // aはmoveされてしまう
    ASSERT_EQ("b", b);  // bはconstなのでmoveされない
    // @@@ sample end
}
}  // namespace
}  // namespace GenVectorUniversalRef_Worst

namespace {
// @@@ sample begin 5:0

template <typename T>
void f(T&& t) noexcept
{
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(t);
    // @@@ ignore end
}
// @@@ sample end

TEST(Template, template_func_only_rvalue)
{
    auto s = std::string{};

    f(s);
    f(std::string{});
}

// @@@ sample begin 5:1

template <typename T>
void f(T&) = delete;
// @@@ sample end

TEST(Template, template_func_only_rvalue2)
{
    // @@@ sample begin 5:2

    auto s = std::string{};

    // f(s);           // f(std::string&)はdeleteされたため、コンパイルエラー
    f(std::string{});  // f(std::string&&)にはバインドできる
    // @@@ sample end
}
}  // namespace
