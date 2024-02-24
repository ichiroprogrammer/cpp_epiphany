#include <list>

#include "gtest_wrapper.h"

namespace RangeFor {
TEST(RangeFor, range_for)
{
    {
        // @@@ sample begin 8:0

        auto list = std::list{1, 2, 3};

        for (auto const a : list) {  // 範囲for文
            std::cout << a << std::endl;
        }

        // 上記for文は下記for文のシンタックスシュガー
        for (std::list<int32_t>::iterator it = std::begin(list); it != std::end(list); ++it) {
            std::cout << *it << std::endl;
        }
        // @@@ sample end
    }

    {
        // @@@ sample begin 8:1

        int32_t array[3]{1, 2, 3};

        for (auto const a : array) {  // 範囲for文
            std::cout << a << std::endl;
        }

        // 上記for文は下記for文のシンタックスシュガー
        for (int32_t* it = std::begin(array); it != std::end(array); ++it) {
            std::cout << *it << std::endl;
        }
        // @@@ sample end
    }
}
}  // namespace RangeFor
