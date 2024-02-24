#include "widget_ok_internal.h"

namespace WidgetOK {
// @@@ sample begin 0:0

// widget_pimpl
void Widget::widget_pimpl::DoSomething()
{
    // 何らかの処理
    xxx_data_ = gen_xxx_data(xxx_data_);
}

uint32_t Widget::widget_pimpl::gen_xxx_data(uint32_t a) { return a * 3; }

// Widget
void     Widget::DoSomething() { widget_pimpl_->DoSomething(); }
uint32_t Widget::GetValue() const { return widget_pimpl_->xxx_data_; }

// ヘッダファイルの中では、widget_pimplは不完全型であるため、コンストラクタ、
// デストラクタは下記に定義する
Widget::Widget() : widget_pimpl_{std::make_unique<Widget::widget_pimpl>()} {}
Widget::~Widget() = default;
// @@@ sample end
}  // namespace WidgetOK
