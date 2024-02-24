#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace WrongCode {
// @@@ sample begin 0:0

namespace App {

struct XY {
    int x;
    int y;
};
// @@@ sample end
// @@@ sample begin 0:1

// このような関数テンプレートは適用範囲が広すぎるので定義すべきではないが、
// 危険な例を示すためあえて定義している
template <typename T, typename U>
inline auto is_equal(T const& lhs, U const& rhs) noexcept
    -> decltype(lhs.x == rhs.x, lhs.y == rhs.y)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
}  // namespace App
// @@@ sample end

namespace {

TEST(Template, suppress_adl_0)
{
    // @@@ sample begin 0:2

    auto xy0 = App::XY{0, 1};
    auto xy1 = App::XY{0, 2};
    auto xy2 = App::XY{0, 1};
    // @@@ sample end
    // @@@ sample begin 0:3

    ASSERT_FALSE(is_equal(xy0, xy1));
    ASSERT_TRUE(is_equal(xy0, xy2));
    // @@@ sample end
    // @@@ sample begin 0:4

    struct point {
        int x;
        int y;
    };
    auto p0 = point{0, 1};

    // 下記のような比較ができるようにするためis_equalはテンプレートで実装している
    ASSERT_TRUE(is_equal(p0, xy0));
    ASSERT_FALSE(is_equal(p0, xy1));
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 1:0

namespace App2 {
struct XYZ {
    int x;
    int y;
    int z;
};

inline bool is_equal(XYZ const& lhs, XYZ const& rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
}  // namespace App2
// @@@ sample end

namespace {

TEST(Template, suppress_adl_1)
{
    {
        // @@@ sample begin 1:1
        auto xyz0 = App2::XYZ{0, 2, 2};
        auto xyz1 = App2::XYZ{0, 1, 2};

        ASSERT_TRUE(is_equal(xyz0, xyz0));
        ASSERT_FALSE(is_equal(xyz0, xyz1));
        // @@@ sample end
    }
    {
        // @@@ sample begin 1:2

        auto xyz0 = App2::XYZ{0, 2, 2};
        auto xyz1 = App2::XYZ{0, 1, 2};
        auto xy0  = App::XY{0, 1};

        ASSERT_FALSE(is_equal(xy0, xyz0));  // これがコンパイルできてしまう
        ASSERT_TRUE(is_equal(xy0, xyz1));   // このis_equalはAppで定義されたもの
        // @@@ sample end
    }
}
}  // namespace
}  // namespace WrongCode

namespace MeasuresCode {
// @@@ sample begin 2:0

template <typename T, size_t N>
constexpr auto array_length(T const (&)[N]) noexcept
{
    return N;
}
// @@@ sample end

TEST(Template, array_length)
{
    int         a[3];
    std::string str[15];

    static_assert(array_length(a) == 3);
    static_assert(array_length(str) == 15);
}

// @@@ sample begin 2:1

namespace App {

struct XY {
    int x;
    int y;
};

inline bool is_equal(XY const& lhs, XY const& rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
}  // namespace App
// @@@ sample end

namespace {

TEST(Template, suppress_adl_2)
{
    auto xy0 = App::XY{0, 1};
    auto xy1 = App::XY{0, 2};
    auto xy2 = App::XY{0, 1};

    ASSERT_FALSE(is_equal(xy0, xy1));
    ASSERT_TRUE(is_equal(xy0, xy2));

    // @@@ sample begin 2:2

    // 下記のpointのようなクラスが他にもいくつかあった場合、
    // このジェネリックラムダでコードの被りは回避できる
    auto is_equal = [](auto const& lhs, auto const& rhs) noexcept {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    };

    struct point {
        int x;
        int y;
    };
    auto p0 = point{0, 1};

    ASSERT_TRUE(is_equal(p0, xy0));
    ASSERT_FALSE(is_equal(p0, xy1));
    // @@@ sample end
}
}  // namespace
}  // namespace MeasuresCode

namespace QualifiedName_1 {

// @@@ sample begin 3:0

struct A {
    int f(int i) noexcept { return i * 3; }
};

int f(int i) noexcept { return i * 2; }

namespace App {

template <typename T>
class ExecF : public T {
public:
    int operator()(int i) noexcept
    {
        return f(i);  // T::fの呼び出しにも見えるが、::fの呼び出し
    }

    // Tを使ったコード
    // @@@ ignore begin
    // @@@ ignore end
};
}  // namespace App
// @@@ sample end

TEST(Template, qualified_name_1)
{
    // @@@ sample begin 3:1

    auto ef = App::ExecF<A>{};

    ASSERT_EQ(4, ef(2));  // ::fの呼び出しなので、2 * 2 == 4となる
    // @@@ sample end
}
}  // namespace QualifiedName_1

namespace QualifiedName_1_refactored {
// @@@ sample begin 4:0

struct A {
    int f(int i) noexcept { return i * 3; }
};

int f(int i) noexcept { return i * 2; }

namespace App {

class ExecF : public A {
public:
    int operator()(int i) noexcept { return f(i); }

    // Tを使ったコード
    // @@@ ignore begin
    // @@@ ignore end
};
}  // namespace App
// @@@ sample end

TEST(Template, qualified_name_1_refactored)
{
    // @@@ sample begin 4:1

    auto ef = App::ExecF{};

    // ASSERT_EQ(4, ef(2));
    ASSERT_EQ(6, ef(2));  // リファクタリングでname lookupの対象が変わり、A::fが呼ばれる
    // @@@ sample end
}
}  // namespace QualifiedName_1_refactored
