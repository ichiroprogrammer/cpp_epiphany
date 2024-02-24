#include <sstream>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_put_to.h"

namespace NoFirewall {
// @@@ sample begin 0:0

namespace App {

template <typename T>
std::string ToString(std::vector<T> const& t)
{
    auto oss = std::ostringstream{};

    using Nstd::operator<<;
    oss << t;  // Nstd::operator<<もname lookupの対象に含める

    return oss.str();
}
}  // namespace App

// @@@ ignore begin
// @@@ ignore end

namespace App {
struct XY {
    XY(int ax, int ay) noexcept : x{ax}, y{ay} {}
    int x;
    int y;
};

std::ostream& operator<<(std::ostream& os, XY const& xyz)
{
    return os << std::to_string(xyz.x) + "/" + std::to_string(xyz.y);
}
}  // namespace App
// @@@ sample end

namespace {

TEST(Template, adl_firewall_0)
{
    // @@@ sample begin 0:1

    auto xys = std::vector<App::XY>{{9, 8}, {7, 6}};

    ASSERT_EQ("9/8, 7/6", App::ToString(xys));
    // @@@ sample end
}
}  // namespace
}  // namespace NoFirewall
