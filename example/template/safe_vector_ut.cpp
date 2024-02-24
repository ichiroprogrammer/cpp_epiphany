#include <type_traits>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_type_traits.h"
#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace Nstd {

template <typename T>
struct SafeVector : std::vector<T> {
    using std::vector<T>::vector;  // 継承コンストラクタ

    using base_type = std::vector<T>;
    using size_type = typename base_type::size_type;

    typename base_type::reference       operator[](size_type i) { return this->at(i); }
    typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
};
}  // namespace Nstd
// @@@ sample end

namespace {
TEST(Template, safe_vector)
{
    // @@@ sample begin 0:1

    {
        auto v = Nstd::SafeVector<int>(10);  // ()での初期化

        ASSERT_EQ(10, v.size());
    }
    {
        auto const v = Nstd::SafeVector<int>{10};

        ASSERT_EQ(1, v.size());
        ASSERT_EQ(10, v[0]);
        ASSERT_THROW(v[1], std::out_of_range);  // エクセプションの発生
    }
    {
        auto v = Nstd::SafeVector<std::string>{"1", "2", "3"};

        ASSERT_EQ(3, v.size());
        ASSERT_EQ((std::vector<std::string>{"1", "2", "3"}), v);
        ASSERT_THROW(v[3], std::out_of_range);  // エクセプションの発生
    }
    {
        auto const v = Nstd::SafeVector<std::string>{"1", "2", "3"};

        ASSERT_EQ(3, v.size());
        ASSERT_EQ((std::vector<std::string>{"1", "2", "3"}), v);
        ASSERT_THROW(v[3], std::out_of_range);  // エクセプションの発生
    }
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 1:0
namespace Nstd {

struct SafeString : std::string {
    using std::string::string;  // 継承コンストラクタ

    using base_type = std::string;
    using size_type = typename base_type::size_type;

    typename base_type::reference       operator[](size_type i) { return this->at(i); }
    typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
};
}  // namespace Nstd
// @@@ sample end

namespace {
TEST(Template, safe_string)
{
    // @@@ sample begin 1:1

    {
        auto s = Nstd::SafeString{"0123456789"};

        ASSERT_EQ(10, s.size());
        ASSERT_EQ("0123456789", s);
        ASSERT_THROW(s[10], std::out_of_range);
    }
    {
        auto const s = Nstd::SafeString(3, 'c');  // ()での初期化が必要

        ASSERT_EQ(3, s.size());
        ASSERT_EQ("ccc", s);
    }
    // @@@ sample end
}
}  // namespace

namespace NG {

// @@@ sample begin 2:0

namespace Nstd {

template <typename T, size_t N>
struct SafeArray : std::array<T, N> {
    using std::array<T, N>::array;  // 継承コンストラクタ

    using base_type = std::array<T, N>;
    using size_type = typename base_type::size_type;

    typename base_type::reference       operator[](size_type i) { return this->at(i); }
    typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
};
}  // namespace Nstd
// @@@ sample end

namespace {
TEST(Template, safe_array_ng)
{
    {
        // @@@ sample begin 2:1

        auto sa_not_init = Nstd::SafeArray<int, 3>{};

        ASSERT_EQ(3, sa_not_init.size());
        ASSERT_THROW(sa_not_init[3], std::out_of_range);
        // @@@ sample end
    }
    {
#if 0
        // @@@ sample begin 2:2

        // std::initializer_listを引数とするコンストラクタが未定義
        auto sa_init = Nstd::SafeArray<int, 3>{1, 2, 3};

        // デフォルトコンストラクタがないため、未初期化
        Nstd::SafeArray<int, 3> const sa_const;
        // @@@ sample end
#endif
    }
}
}  // namespace
}  // namespace NG

// @@@ sample begin 3:0

namespace Nstd {

template <typename T, size_t N>
struct SafeArray : std::array<T, N> {
    using std::array<T, N>::array;  // 継承コンストラクタ
    using base_type = std::array<T, N>;
    // @@@ sample end
    // @@@ sample begin 3:1

    template <typename... ARGS>  // コンストラクタを定義
    SafeArray(ARGS... args) : base_type{args...}
    {
    }
    // @@@ sample end
    // @@@ sample begin 3:2

    using size_type = typename base_type::size_type;

    typename base_type::reference       operator[](size_type i) { return this->at(i); }
    typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
};
}  // namespace Nstd
// @@@ sample end

namespace {
TEST(Template, safe_array)
{
    {
        auto sa_not_init = Nstd::SafeArray<int, 3>{};

        ASSERT_EQ(3, sa_not_init.size());
        ASSERT_THROW(sa_not_init[3], std::out_of_range);
    }
    // @@@ sample begin 3:3
    {
        auto sa_init = Nstd::SafeArray<int, 3>{1, 2, 3};

        ASSERT_EQ(3, sa_init.size());
        ASSERT_EQ(1, sa_init[0]);
        ASSERT_EQ(2, sa_init[1]);
        ASSERT_EQ(3, sa_init[2]);
        ASSERT_THROW(sa_init[3], std::out_of_range);
    }
    {
        auto const sa_string_const = Nstd::SafeArray<std::string, 5>{"1", "2", "3"};

        ASSERT_EQ(5, sa_string_const.size());
        ASSERT_EQ("1", sa_string_const[0]);
        ASSERT_EQ("2", sa_string_const[1]);
        ASSERT_EQ("3", sa_string_const[2]);
        ASSERT_EQ("", sa_string_const[3]);
        ASSERT_EQ("", sa_string_const[4]);
        ASSERT_THROW(sa_string_const[5], std::out_of_range);
    }
    // @@@ sample end
    {
        auto       cl              = std::initializer_list<char>{'1', '2', '3'};
        auto const sa_string_const = Nstd::SafeArray<std::string, 2>{std::string{"aaa"}, cl};

        ASSERT_EQ(2, sa_string_const.size());
        ASSERT_EQ("aaa", sa_string_const[0]);
        ASSERT_EQ("123", sa_string_const[1]);
        ASSERT_THROW(sa_string_const[2], std::out_of_range);
    }
#if 0
    // @@@ sample begin 3:4
    {
        auto sa_init = Nstd::SafeArray<int, 3>{1.0, 2, 3};

        ASSERT_EQ(3, sa_init.size());
        ASSERT_EQ(1, sa_init[0]);
        ASSERT_EQ(2, sa_init[1]);
        ASSERT_EQ(3, sa_init[2]);
        ASSERT_THROW(sa_init[3], std::out_of_range);
    }
    // @@@ sample end
#endif
}
}  // namespace

// @@@ sample begin 4:0
namespace Nstd {

template <typename T, size_t N>
struct SafeArray2 : std::array<T, N> {
    using std::array<T, N>::array;  // 継承コンストラクタ
    using base_type = std::array<T, N>;
    // clang-format off
    // @@@ sample end
    // @@@ sample begin 4:1

    // 縮小型変換した場合には、ill-formedになるコンストラクタ
    template <typename... ARGS,
              typename = 
                  typename std::enable_if_t<
                       AreConvertibleWithoutNarrowConvV<T, ARGS...>>>
    SafeArray2(ARGS... args) : base_type{args...} // 初期化子リストによるarrayの初期化
    {
    }
    // @@@ sample end
    // @@@ sample begin 4:2

    // 縮小型変換しない場合には、ill-formedになるコンストラクタ
    template <typename... ARGS, 
              typename std::enable_if_t<
                  !AreConvertibleWithoutNarrowConvV<T, ARGS...>>* = nullptr>
    SafeArray2(ARGS... args) :
        // @@@ sample end
        // @@@ sample begin 4:3
        base_type{T(args)...},  // 縮小型変換を抑止するため、T(args)が必要
        // @@@ sample end
        // @@@ sample begin 4:4
        is_with_narrow_conv_{true}
    {
    }
    // @@@ sample end
    // @@@ sample begin 4:5
    // clang-format on

    using size_type = typename base_type::size_type;

    typename base_type::reference       operator[](size_type i) { return this->at(i); }
    typename base_type::const_reference operator[](size_type i) const { return this->at(i); }

    bool InitializedWithNarrowConv() const noexcept { return is_with_narrow_conv_; }

private:
    bool const is_with_narrow_conv_{false};
};
}  // namespace Nstd
// @@@ sample end

namespace {
TEST(Template, safe_array2)
{
    {
        auto sa_not_init = Nstd::SafeArray2<int, 3>{};

        ASSERT_FALSE(sa_not_init.InitializedWithNarrowConv());
        ASSERT_EQ(3, sa_not_init.size());
        ASSERT_THROW(sa_not_init[3], std::out_of_range);
    }
    {
        // @@@ sample begin 4:6

        auto sa_init = Nstd::SafeArray2<int, 3>{1, 2, 3};
        // @@@ sample end
        // @@@ sample begin 4:7

        ASSERT_FALSE(sa_init.InitializedWithNarrowConv());  // 縮小型変換なし
        ASSERT_EQ(3, sa_init.size());
        ASSERT_EQ(1, sa_init[0]);
        ASSERT_EQ(2, sa_init[1]);
        ASSERT_EQ(3, sa_init[2]);
        ASSERT_THROW(sa_init[3], std::out_of_range);
        // @@@ sample end
    }
    {
        // @@@ sample begin 4:8
        auto const sa_init = Nstd::SafeArray2<int, 3>{10, 20, 30.0};  // 30.0はintに縮小型変換される

        ASSERT_TRUE(sa_init.InitializedWithNarrowConv());  // 縮小型変換あり
        ASSERT_EQ(3, sa_init.size());
        ASSERT_EQ(10, sa_init[0]);
        ASSERT_EQ(20, sa_init[1]);
        ASSERT_EQ(30, sa_init[2]);
        ASSERT_THROW(sa_init[3], std::out_of_range);
        // @@@ sample end
    }
    {
        auto sa_init = Nstd::SafeArray2<int, 4>{2, 3.0};

        ASSERT_TRUE(sa_init.InitializedWithNarrowConv());
        ASSERT_EQ(4, sa_init.size());
        ASSERT_EQ(2, sa_init[0]);
        ASSERT_EQ(3, sa_init[1]);
        ASSERT_EQ(0, sa_init[2]);
        ASSERT_EQ(0, sa_init[3]);
        ASSERT_THROW(sa_init[4], std::out_of_range);
    }
    {
        auto const sa_string_const = Nstd::SafeArray2<std::string, 5>{"1", "2", "3"};

        ASSERT_FALSE(sa_string_const.InitializedWithNarrowConv());
        ASSERT_EQ(5, sa_string_const.size());
        ASSERT_EQ("1", sa_string_const[0]);
        ASSERT_EQ("2", sa_string_const[1]);
        ASSERT_EQ("3", sa_string_const[2]);
        ASSERT_EQ("", sa_string_const[3]);
        ASSERT_EQ("", sa_string_const[4]);
        ASSERT_THROW(sa_string_const[5], std::out_of_range);
    }
    {
        auto       cl              = std::initializer_list<char>{'1', '2', '3'};
        auto const sa_string_const = Nstd::SafeArray2<std::string, 2>{std::string("aaa"), cl};

        ASSERT_FALSE(sa_string_const.InitializedWithNarrowConv());
        ASSERT_EQ(2, sa_string_const.size());
        ASSERT_EQ("aaa", sa_string_const[0]);
        ASSERT_EQ("123", sa_string_const[1]);
        ASSERT_THROW(sa_string_const[2], std::out_of_range);
    }
}
}  // namespace
