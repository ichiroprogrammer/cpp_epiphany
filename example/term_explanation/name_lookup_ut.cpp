#include "gtest_wrapper.h"

#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace NS_LU {
int f() noexcept { return 0; }
}  // namespace NS_LU
// @@@ sample end
// @@@ sample begin 0:1

namespace NS_LU {
bool g(int i) noexcept { return i < 0; }

char g(std::string_view str) noexcept { return str[0]; }

template <typename T, size_t N>
size_t g(T const (&)[N]) noexcept
{
    return N;
}
// @@@ sample end
}  // namespace NS_LU

namespace {
TEST(Lookup, lookup)
{
    // @@@ sample begin 1:0

    NS_LU::f();
    // @@@ sample end

    NS_LU::g(1);
    NS_LU::g(std::string{"123"});

    // @@@ sample begin 1:1
    int a[3]{1, 2, 3};
    NS_LU::g(a);
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 2:0

// グローバル名前空間
std::string ToString(int i) { return std::to_string(i) + " in Global"; }

namespace NS_LU {
struct X {
    int i;
};

std::string ToString(X const& x) { return std::to_string(x.i) + " in NS_LU"; }
}  // namespace NS_LU

namespace NS2 {
std::string ToString(NS_LU::X const& x) { return std::to_string(x.i) + " in NS2"; }
}  // namespace NS2
// @@@ sample end

namespace {
TEST(Lookup, lookup_adl)
{
    // @@@ sample begin 2:1

    auto x = NS_LU::X{1};

    ASSERT_EQ("1 in NS_LU", ToString(x));
    // @@@ sample end
    ASSERT_EQ("1 in NS2", NS2::ToString(x));
    ASSERT_EQ("1 in Global", ToString(x.i));
}
}  // namespace
