#pragma once

#include "widget_ok.h"
// @@@ sample begin 0:0

namespace WidgetOK {  // @@@ delete
struct Widget::widget_pimpl {
    void     DoSomething();
    uint32_t gen_xxx_data(uint32_t a);
    uint32_t xxx_data_{1};
};
// @@@ sample end
}  // namespace WidgetOK
