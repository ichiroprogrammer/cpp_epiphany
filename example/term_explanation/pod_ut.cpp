#include "gtest_wrapper.h"

namespace {
TEST(Pod, pod)
{
    {
        // @@@ sample begin 0:0

        static_assert(std::is_pod<int>::value, "");
        static_assert(std::is_pod<int const>::value, "");
        static_assert(std::is_pod<int*>::value, "");
        static_assert(std::is_pod<int[3]>::value, "");
        static_assert(!std::is_pod<int&>::value, "");  // リファレンスはPODではない

        struct Pod {};

        static_assert(std::is_pod<Pod>::value, "");
        static_assert(std::is_pod<Pod const>::value, "");
        static_assert(std::is_pod<Pod*>::value, "");
        static_assert(std::is_pod<Pod[3]>::value, "");
        static_assert(!std::is_pod<Pod&>::value, "");

        struct NonPod {  // コンストラクタがあるためPODではない
            NonPod();
        };

        static_assert(!std::is_pod<NonPod>::value, "");
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:1

        static_assert(std::is_standard_layout<int>::value, "");
        static_assert(std::is_standard_layout<int*>::value, "");
        static_assert(std::is_standard_layout<int[1]>::value, "");
        static_assert(!std::is_standard_layout<int&>::value, "");

        enum class SizeUndefined { su_0, su_1 };

        struct StanderdLayout {  // 標準レイアウトだがトリビアルではない
            StanderdLayout() : a{0}, b{SizeUndefined::su_0} {}
            int           a;
            SizeUndefined b;
        };

        static_assert(std::is_standard_layout<StanderdLayout>::value, "");
        static_assert(!std::is_trivial<StanderdLayout>::value, "");
        static_assert(!std::is_pod<StanderdLayout>::value, "");
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:2

        static_assert(std::is_trivial<int>::value, "");
        static_assert(std::is_trivial<int*>::value, "");
        static_assert(std::is_trivial<int[1]>::value, "");
        static_assert(!std::is_trivial<int&>::value, "");

        enum class SizeUndefined { su_0, su_1 };

        struct Trivial {      // トリビアルだが標準レイアウトではない
            int&          a;  // リファレンスは標準レイアウトではない
            SizeUndefined b;
        };

        static_assert(!std::is_standard_layout<Trivial>::value, "");
        static_assert(std::is_trivial<Trivial>::value, "");
        static_assert(!std::is_pod<Trivial>::value, "");
        // @@@ sample end
    }
}
}  // namespace
