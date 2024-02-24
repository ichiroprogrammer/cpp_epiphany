#include <sstream>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_put_to.h"

namespace Firewall {
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
// @@@ sample begin 0:0

// Appの中の新たな名前空間XY_Firewall_でstruct XYとoperator<<を宣言
namespace App {
namespace XY_Firewall_ {

struct XY {
    XY(int ax, int ay) noexcept : x{ax}, y{ay} {}
    int x;
    int y;
};

std::ostream& operator<<(std::ostream& os, XY const& xyz)
{
    return os << std::to_string(xyz.x) + "/" + std::to_string(xyz.y);
}
}  // namespace XY_Firewall_

using XY = XY_Firewall_::XY;

}  // namespace App
// @@@ sample end

// Appに下記を追加
namespace App {
template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& t)
{
    return os << "size:" << t.size();
}
}  // namespace App

namespace {

TEST(Template, adl_firewall_1)
{
    auto xys = std::vector<App::XY>{{9, 8}, {7, 6}};

    ASSERT_EQ("9/8, 7/6", App::ToString(xys));
}
}  // namespace
}  // namespace Firewall
