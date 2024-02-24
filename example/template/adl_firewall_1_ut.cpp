#include <sstream>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_put_to.h"

namespace NoFirewall2 {
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

// @@@ sample begin 0:0

// Appに下記を追加
namespace App {
template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& t)
{
    return os << "size:" << t.size();
}
}  // namespace App
// @@@ sample end

namespace {

TEST(Template, adl_firewall_1)
{
    // @@@ sample begin 0:1

    auto xys = std::vector<App::XY>{{9, 8}, {7, 6}};

    // App::operator<<の追加で、App::ToStringの出力が影響を受ける
    // ASSERT_EQ("9/8, 7/6", App::ToString(xys));
    ASSERT_EQ("size:2", App::ToString(xys));
    // @@@ sample end
}
}  // namespace
}  // namespace NoFirewall2
