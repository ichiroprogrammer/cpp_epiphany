#pragma once

#if defined(__clang__)
_Pragma("clang diagnostic push");
_Pragma("clang diagnostic ignored \"-Wsign-compare\"");
#else
_Pragma("GCC diagnostic push");
_Pragma("GCC diagnostic ignored \"-Wsign-compare\"");
_Pragma("GCC diagnostic ignored \"-Weffc++\"");
#endif

// @@@ sample begin 0:0
#include "gtest/gtest.h"

namespace WidgetNG {  // @@@ delete
class Widget {
public:
    void     DoSomething();
    uint32_t GetValue() const;
    // 何らかの宣言

private:
    uint32_t gen_xxx_data(uint32_t a);
    uint32_t xxx_data_{1};
    // @@@ sample end

    // @@@ sample begin 0:1
    FRIEND_TEST(Pimpl, widget_ng);  // 単体テストをfriendにする
    // @@@ sample end
    // @@@ sample begin 0:2
};
// @@@ sample end
}  // namespace WidgetNG
#if defined(__clang__)
_Pragma("clang diagnostic pop");
#else
_Pragma("GCC diagnostic pop");
#endif
