#include <cassert>

#include "gtest_wrapper.h"

#include "app_ints.h"
#include "logger_0.h"
#include "logger_ut.h"
#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace Logging {  // operator<<の定義をLoggingで行う
std::ostream& operator<<(std::ostream& os, App::Ints_t const& ints)
{
    auto first = true;

    for (auto const i : ints) {
        if (!std::exchange(first, false)) {
            os << ", ";
        }
        os << i;
    }

    return os;
}
}  // namespace Logging
// @@@ sample end

#if defined(__clang__)
#define SKIP_COMPILE
#else
#undef SKIP_COMPILE
#endif

#ifndef SKIP_COMPILE
// clang-format off
#define INTS_UT_NAME logger_0_Ints_Logging
#include "logger_0_ints_ut.h"
// clang-format on
#endif
