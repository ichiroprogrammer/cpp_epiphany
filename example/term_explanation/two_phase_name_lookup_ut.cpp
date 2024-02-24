#include "gtest_wrapper.h"

#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace NS_TPLU {
struct X {
    int i;
};
}  // namespace NS_TPLU

// グローバル名前空間
inline std::string ToType(NS_TPLU::X const&) { return "X in global"; }
inline std::string ToType(int const&) { return "int in global"; }

// 再びNS_TPLU
namespace NS_TPLU {

std::string Header(long) { return "type:"; }  //  下記にもオーバーロードあり

template <typename T>
std::string ToType(T const&)  //  下記にもオーバーロードあり
{
    return "unknown";
}

template <typename T>
std::string TypeName(T const& t)  // オーバーロードなし
{
    return Header(int{}) + ToType(t);
}

std::string Header(int) { return "TYPE:"; }  // 上記にもオーバーロードあり

std::string ToType(X const&) { return "X"; }      // 上記にもオーバーロードあり
std::string ToType(int const&) { return "int"; }  // 上記にもオーバーロードあり
}  // namespace NS_TPLU
// @@@ sample end

namespace {
TEST(Lookup, two_phase_name_lookup)
{
    // @@@ sample begin 0:1

    auto x = NS_TPLU::X{1};

    ASSERT_EQ("type:X", TypeName(x));
    // @@@ sample end
    // @@@ sample begin 0:2

    ASSERT_EQ("type:unknown", NS_TPLU::TypeName(int{}));
    // @@@ sample end
    // @@@ sample begin 0:3

    ASSERT_EQ("type:long", NS_TPLU::TypeName(long{}));
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 0:4

namespace NS_TPLU {
template <>
std::string ToType<long>(long const&)
{
    return "long";
}
}  // namespace NS_TPLU
// @@@ sample end
// @@@ sample begin 1:0

namespace NS_TPLU2 {
struct Y {
    int i;
};
}  // namespace NS_TPLU2
// @@@ sample end
// @@@ sample begin 1:1

// global名前空間
template <typename T>
std::string ToType(T const&)
{
    return "unknown";
}

template <typename T>
std::string TypeName(T const& t)
{
    return "type:" + ToType(t);
}

std::string ToType(NS_TPLU2::Y const&) { return "Y"; }
// @@@ sample end

namespace {
TEST(Lookup, two_phase_name_lookup2)
{
    // @@@ sample begin 1:2

    auto y = NS_TPLU2::Y{1};

    // ASSERT_EQ("type:Y", TypeName(y));
    ASSERT_EQ("type:unknown", TypeName(y));  // ToType(NS_TPLU2::Y const&)は使われない
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 2:0

// global名前空間
template <typename T>
int operator+(T const&, int i)
{
    return i;
}

template <typename T>
int TypeNum(T const& t)
{
    return t + 0;
}

int operator+(NS_TPLU2::Y const& y, int i) { return y.i + i; }
// @@@ sample end

namespace {
TEST(Lookup, two_phase_name_lookup3)
{
    {
        // @@@ sample begin 2:1

        auto y = NS_TPLU2::Y{1};

        ASSERT_EQ(1, y + 0);  // 2つ目のoperator+が選択される
        // @@@ sample end
    }

#ifndef __clang__  // g++
    {
        // @@@ sample begin 2:2

        auto y = NS_TPLU2::Y{1};

        ASSERT_EQ(1, TypeNum(y));  // g++ではoperator+(NS_TPLU2::Y const&, int i)がname lookupされる
        // @@@ sample end
    }
#else  // clang++
    {
        // @@@ sample begin 2:3

        auto y = NS_TPLU2::Y{1};

        ASSERT_EQ(0, TypeNum(y));  // clang++ではoperator+(T const&, int i)がname lookupされる
        // @@@ sample end
    }
#endif
}
}  // namespace
