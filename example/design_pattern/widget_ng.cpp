#include "widget_ng.h"

namespace WidgetNG {
// @@@ sample begin 0:0

void Widget::DoSomething()
{
    // 何らかの処理
    xxx_data_ = gen_xxx_data(xxx_data_);
}

uint32_t Widget::GetValue() const { return xxx_data_; }

uint32_t WidgetNG::Widget::gen_xxx_data(uint32_t a) { return a * 3; }
// @@@ sample end
}  // namespace WidgetNG
