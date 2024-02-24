#include <cassert>
#include <vector>

#include "gtest_wrapper.h"

#include "logger_0.h"
#include "logger_ut.h"
#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace App {                     // Ints_tの宣言はApp
struct Ints_t : std::vector<int> {  // エイリアスではなく、継承を使う
    using vector::vector;           // 継承コンストラクタ
};

// App内
std::ostream& operator<<(std::ostream& os, Ints_t const& ints)
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
}  // namespace App
// @@@ sample end

// clang-format off
#define INTS_UT_NAME logger_0_Ints_struct
#include "logger_0_ints_ut.h"
// clang-format on
