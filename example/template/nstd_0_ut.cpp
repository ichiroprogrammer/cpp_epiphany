#include <sstream>

#include "gtest_wrapper.h"

#include "app_ints.h"
#include "nstd_0.h"
#include "suppress_warning.h"

namespace {

TEST(Template, Nstd_put_to)
{
    // @@@ sample begin 0:0

    auto const ints = App::Ints_t{1, 2, 3};

    {
        auto oss = std::ostringstream{};

        using namespace Nstd;
        oss << ints << 4;
        ASSERT_EQ("1, 2, 34", oss.str());
    }
    {
        auto oss = std::ostringstream{};

        Nstd::operator<<(oss, ints) << 4;  // 念のためこの形式でもテスト
        ASSERT_EQ("1, 2, 34", oss.str());
    }

    ASSERT_EQ("1, 2, 3", Nstd::ToString(ints));
    // @@@ sample end
}
}  // namespace
