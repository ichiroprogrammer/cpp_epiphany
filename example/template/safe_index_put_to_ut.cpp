#include "gtest_wrapper.h"

#include "nstd_safe_index.h"

namespace SafeIndexPutToNG {
namespace {

// @@@ sample begin 0:0

template <template <class...> class C, typename... Ts>
std::ostream& operator<<(std::ostream& os, Nstd::SafeIndex<C, Ts...> const& safe_index)
{
    auto first = true;

    for (auto const& i : safe_index) {
        if (!std::exchange(first, false)) {
            os << ", ";
        }
        os << i;
    }

    return os;
}
// @@@ sample end

TEST(Template, safe_index_put_to_ng)
{
    // @@@ sample begin 1:0
    {
        auto v_i = Nstd::SafeVector<int>{1, 2};

        auto oss = std::ostringstream{};
        oss << v_i;
        ASSERT_EQ("1, 2", oss.str());
    }
    {
        auto sal_s = Nstd::SafeArray<std::string, 4>{"1", "2", "3"};
        auto oss   = std::ostringstream{};
        oss << sal_s;
        ASSERT_EQ("1, 2, 3, ", oss.str());  // 4番目には何も入っていない
    }
    // @@@ sample end
    // @@@ sample begin 1:1

    {
        auto s_str = Nstd::SafeString{"hello"};
        auto oss   = std::ostringstream{};
        oss << s_str;

        // ASSERT_EQ("hello", oss.s_str());     // これがパス出来たらよいが、
        ASSERT_EQ("h, e, l, l, o", oss.str());  // 実際にはこのようになる。
    }
    {
        auto str = std::string{"hello"};  // 上記と比較のためのstd::stringでのoperator<<

        auto oss = std::ostringstream{};
        oss << str;
        ASSERT_EQ("hello", oss.str());
    }
    // @@@ sample end

    {
        auto u16str = Nstd::SafeStringU16{u"あいうえお"};
        auto oss    = std::basic_ostringstream<char16_t>{};
        oss << u16str;
        ASSERT_EQ(u"あいうえお", oss.str());
    }
    {
        auto u32str = Nstd::SafeStringU32{U"あいうえお"};
        auto oss    = std::basic_ostringstream<char32_t>{};
        oss << u32str;
        ASSERT_EQ(U"あいうえお", oss.str());
    }
}
}  // namespace
}  // namespace SafeIndexPutToNG

namespace SafeIndexPutToOK {
namespace {

template <typename T>
constexpr bool is_kind_of_safe_string_v{
    Nstd::IsSameSomeOfV<typename std::remove_const_t<T>, Nstd::SafeString, Nstd::SafeStringU16,
                        Nstd::SafeStringU32>};

TEST(Template, is_kind_of_safe_string)
{
    static_assert(!is_kind_of_safe_string_v<std::string>);
    static_assert(is_kind_of_safe_string_v<Nstd::SafeString>);
    static_assert(is_kind_of_safe_string_v<Nstd::SafeString const>);

    static_assert(!is_kind_of_safe_string_v<std::u16string>);
    static_assert(is_kind_of_safe_string_v<Nstd::SafeStringU16>);
    static_assert(is_kind_of_safe_string_v<Nstd::SafeStringU16 const>);

    static_assert(!is_kind_of_safe_string_v<std::u32string>);
    static_assert(is_kind_of_safe_string_v<Nstd::SafeStringU32>);
    static_assert(is_kind_of_safe_string_v<Nstd::SafeStringU32 const>);
}

// clang-format off
// @@@ sample begin 2:0

template <template <class...> class C, typename... Ts>
auto operator<<(std::ostream& os, Nstd::SafeIndex<C, Ts...> const& safe_index) ->
    typename std::enable_if_t<    // safe_indexがSafeString型ならば、SFINAEにより非活性化
        !std::is_same_v<Nstd::SafeIndex<C, Ts...>, Nstd::SafeString>, std::ostream&>
// clang-format on
{
    auto first = true;

    for (auto const& i : safe_index) {
        if (!std::exchange(first, false)) {
            os << ", ";
        }
        os << i;
    }

    return os;
}
// @@@ sample end

TEST(Template, safe_index_put_to_ok)
{
    {
        auto v_i = Nstd::SafeVector<int>{1, 2};
        auto oss = std::ostringstream{};
        oss << v_i;
        ASSERT_EQ("1, 2", oss.str());
    }
    {
        auto sal_s = Nstd::SafeArray<std::string, 4>{"1", "2", "3"};
        auto oss   = std::ostringstream{};
        oss << sal_s;
        ASSERT_EQ("1, 2, 3, ", oss.str());
    }
    {
        // @@@ sample begin 3:0

        auto str = Nstd::SafeString{"hello"};
        auto oss = std::ostringstream{};
        oss << str;
        ASSERT_EQ("hello", oss.str());  // std::operator<<が使われる
        // ASSERT_EQ("h, e, l, l, o", oss.str());
        // @@@ sample end
    }
    {
        auto u16str = Nstd::SafeStringU16{u"あいうえお"};
        auto oss    = std::basic_ostringstream<char16_t>{};
        oss << u16str;
        ASSERT_EQ(u"あいうえお", oss.str());
    }
    {
        auto u32str = Nstd::SafeStringU32{U"あいうえお"};
        auto oss    = std::basic_ostringstream<char32_t>{};
        oss << u32str;
        ASSERT_EQ(U"あいうえお", oss.str());
    }
}
}  // namespace
}  // namespace SafeIndexPutToOK
