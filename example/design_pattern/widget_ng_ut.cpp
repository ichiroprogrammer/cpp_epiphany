#include "widget_ng.h"

namespace WidgetNG {
// @@@ sample begin 0:0

TEST(Pimpl, widget_ng)
{
    Widget w;

    ASSERT_EQ(1, w.xxx_data_);  // privateのテスト
    w.DoSomething();
    ASSERT_EQ(3, w.xxx_data_);        // privateのテスト
    ASSERT_EQ(9, w.gen_xxx_data(3));  // privateのテスト

    ASSERT_EQ(3, w.GetValue());
}
// @@@ sample end
}  // namespace WidgetNG
