#include "gtest_wrapper.h"

#include "nstd_safe_index.h"

namespace {
TEST(Template, safe_index)
{
    // @@@ sample begin 0:0

    auto v_i = Nstd::SafeIndex<std::vector, int>{1, 2};

    static_assert(std::is_same_v<int&, decltype(v_i[0])>);
    static_assert(std::is_base_of_v<std::vector<int>, decltype(v_i)>);
    ASSERT_EQ(1, v_i[0]);
    ASSERT_EQ(2, v_i[1]);
    ASSERT_THROW(v_i[2], std::out_of_range);

    auto str = Nstd::SafeIndex<std::basic_string, char>{"123"};

    static_assert(std::is_same_v<char&, decltype(str[0])>);
    static_assert(std::is_base_of_v<std::string, decltype(str)>);
    ASSERT_EQ(3, str.size());
    ASSERT_EQ("123", str);
    ASSERT_THROW(str[3], std::out_of_range);
    // @@@ sample end

    auto const c5 = Nstd::SafeIndex<std::basic_string, char>(5, 'c');  // ()で初期化が必要

    static_assert(std::is_same_v<char const&, decltype(c5[0])>);
    static_assert(std::is_base_of_v<std::string, decltype(str)>);
    ASSERT_EQ(5, c5.size());
    ASSERT_EQ("ccccc", c5);
    ASSERT_THROW(c5[5], std::out_of_range);

    auto const v_s = Nstd::SafeIndex<std::vector, std::string>{3};

    static_assert(std::is_same_v<std::string const&, decltype(v_s[0])>);
    static_assert(std::is_base_of_v<std::vector<std::string>, decltype(v_s)>);
    ASSERT_EQ(3, v_s.size());
    ASSERT_THROW(v_s[3], std::out_of_range);
    ASSERT_EQ((std::vector<std::string>{"", "", ""}), v_s);

#if 0
    // @@@ sample begin 0:1

    // 下記のように書きたいが、パラメータパックは型と値を混在できないのでコンパイルエラー
    auto a_i = Nstd::SafeIndex<std::array, int, 5>{};
    // @@@ sample end
#endif
}

TEST(Template, safe_index_alias)
{
    // @@@ sample begin 1:0

    auto v_i = Nstd::SafeVector<int>{1, 2};

    static_assert(std::is_same_v<int&, decltype(v_i[0])>);
    static_assert(std::is_base_of_v<std::vector<int>, decltype(v_i)>);
    ASSERT_EQ(1, v_i[0]);
    ASSERT_EQ(2, v_i[1]);
    ASSERT_THROW(v_i[2], std::out_of_range);

    auto str = Nstd::SafeString{"123"};

    static_assert(std::is_same_v<char&, decltype(str[0])>);
    static_assert(std::is_base_of_v<std::string, decltype(str)>);
    ASSERT_EQ(3, str.size());
    ASSERT_EQ("123", str);
    ASSERT_THROW(str[3], std::out_of_range);
    // @@@ sample end

    auto const v_s = Nstd::SafeVector<std::string>{3};

    static_assert(std::is_same_v<std::string const&, decltype(v_s[0])>);
    static_assert(std::is_base_of_v<std::vector<std::string>, decltype(v_s)>);
    ASSERT_EQ(3, v_s.size());
    ASSERT_EQ((std::vector<std::string>{"", "", ""}), v_s);
    ASSERT_THROW(v_s[3], std::out_of_range);

    auto const c5 = Nstd::SafeString(5, 'c');  // ()での初期化が必要

    static_assert(std::is_same_v<char const&, decltype(c5[0])>);
    static_assert(std::is_base_of_v<std::string, decltype(c5)>);
    ASSERT_EQ(5, c5.size());
    ASSERT_EQ("ccccc", c5);
    ASSERT_THROW(c5[5], std::out_of_range);

    // @@@ sample begin 1:1

    auto sal_s = Nstd::SafeArray<std::string, 4>{"1", "2", "3"};

    static_assert(std::is_same_v<std::string&, decltype(sal_s[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 4>, decltype(sal_s)>);
    ASSERT_EQ("1", sal_s[0]);
    ASSERT_EQ("2", sal_s[1]);
    ASSERT_EQ("3", sal_s[2]);
    ASSERT_EQ("", sal_s[3]);
    ASSERT_THROW(sal_s[4], std::out_of_range);
    // @@@ sample end

    auto const sal_s2 = sal_s;

    static_assert(std::is_same_v<std::string const&, decltype(sal_s2[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 4>, decltype(sal_s2)>);
    ASSERT_EQ("1", sal_s2[0]);
    ASSERT_EQ("2", sal_s2[1]);
    ASSERT_EQ("3", sal_s2[2]);
    ASSERT_EQ("", sal_s2[3]);
    ASSERT_THROW(sal_s2[4], std::out_of_range);

    // @@@ sample begin 1:2

    auto u16str = Nstd::SafeStringU16{u"あいうえお"};

    static_assert(std::is_same_v<char16_t&, decltype(u16str[0])>);
    static_assert(std::is_base_of_v<std::u16string, decltype(u16str)>);
    ASSERT_EQ(5, u16str.size());
    ASSERT_EQ(u"あいうえお", u16str);
    ASSERT_THROW(u16str[5], std::out_of_range);

    auto u32str = Nstd::SafeStringU32{U"かきくけこ"};

    static_assert(std::is_same_v<char32_t&, decltype(u32str[0])>);
    static_assert(std::is_base_of_v<std::u32string, decltype(u32str)>);
    ASSERT_EQ(5, u32str.size());
    ASSERT_EQ(U"かきくけこ", u32str);
    ASSERT_THROW(u32str[5], std::out_of_range);
    // @@@ sample end
}

TEST(Template, std_array_like)
{
    // @@@ sample begin 2:0

    auto sal = Nstd::Inner_::std_array_like<int, std::integral_constant<size_t, 3>>{1, 2, 3};

    static_assert(std::is_nothrow_constructible_v<decltype(sal), int>);  // エクセプション無し生成
    static_assert(std::is_same_v<int&, decltype(sal[0])>);
    static_assert(std::is_base_of_v<std::array<int, 3>, decltype(sal)>);
    ASSERT_EQ(1, sal[0]);
    ASSERT_EQ(2, sal[1]);
    ASSERT_EQ(3, sal[2]);

    using T   = Nstd::Inner_::std_array_like<std::string, std::integral_constant<size_t, 3>>;
    auto sal2 = T{"1", "2", "3"};

    static_assert(!std::is_nothrow_constructible_v<std::string, char const*>);
    static_assert(!std::is_nothrow_constructible_v<T, char const*>);  // エクセプション有り生成
    static_assert(std::is_same_v<std::string&, decltype(sal2[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 3>, decltype(sal2)>);
    ASSERT_EQ("1", sal2[0]);
    ASSERT_EQ("2", sal2[1]);
    ASSERT_EQ("3", sal2[2]);
    // @@@ sample end

    // @@@ sample begin 2:1

    using T2   = Nstd::SafeIndex<Nstd::Inner_::std_array_like, std::string,
                               std::integral_constant<size_t, 4>>;
    auto sal_s = T2{"1", "2", "3"};

    static_assert(!std::is_nothrow_constructible_v<T2, char const*>);  // エクセプション有り生成
    static_assert(std::is_same_v<std::string&, decltype(sal_s[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 4>, decltype(sal_s)>);
    ASSERT_EQ("1", sal_s[0]);
    ASSERT_EQ("2", sal_s[1]);
    ASSERT_EQ("3", sal_s[2]);
    ASSERT_EQ("", sal_s[3]);
    ASSERT_THROW(sal_s[4], std::out_of_range);
    // @@@ sample end

    auto const sal_s2 = sal_s;

    static_assert(std::is_same_v<std::string const&, decltype(sal_s2[0])>);
    static_assert(std::is_base_of_v<std::array<std::string, 4>, decltype(sal_s2)>);
    ASSERT_EQ("1", sal_s2[0]);
    ASSERT_EQ("2", sal_s2[1]);
    ASSERT_EQ("3", sal_s2[2]);
    ASSERT_EQ("", sal_s2[3]);
    ASSERT_THROW(sal_s2[4], std::out_of_range);
}
}  // namespace
