#pragma once

// @@@ sample begin 0:0
#include <memory>

namespace WidgetOK {  // @@@ delete
class Widget {
public:
    Widget();   // widget_pimplは不完全型であるため、コンストラクタ、
    ~Widget();  // デストラクタはインラインにできない
    void     DoSomething();
    uint32_t GetValue() const;
    // 何らかの宣言

    struct widget_pimpl;  // 単体テストのため、publicとするが、実装はsrc/の下に置くため、
                          // 単体テスト以外の外部からのアクセスはできない

private:
    std::unique_ptr<widget_pimpl> widget_pimpl_;
};
// @@@ sample end
}  // namespace WidgetOK
