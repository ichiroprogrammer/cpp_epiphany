#if defined(__clang__)
_Pragma("clang diagnostic push");
_Pragma("clang diagnostic ignored \"-Wsign-compare\"");
#else
_Pragma("GCC diagnostic push");
_Pragma("GCC diagnostic ignored \"-Wsign-compare\"");
_Pragma("GCC diagnostic ignored \"-Weffc++\"");
#endif

#include "widget_ok_internal.h"
// @@@ sample begin 0:0
#include "gtest/gtest.h"

namespace WidgetOK {  // @@@ delete
TEST(Pimpl, widget_ok)
{
    Widget::widget_pimpl wi;

    ASSERT_EQ(1, wi.xxx_data_);
    wi.DoSomething();
    ASSERT_EQ(3, wi.xxx_data_);
    ASSERT_EQ(9, wi.gen_xxx_data(3));

    Widget w;

    w.DoSomething();
    ASSERT_EQ(3, w.GetValue());
}
// @@@ sample end
}  // namespace WidgetOK
