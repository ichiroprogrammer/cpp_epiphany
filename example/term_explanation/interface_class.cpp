#include <cstdint>

#include "suppress_warning.h"

namespace {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_UNUSED_FUNC;
// @@@ sample begin 0:0

class InterfaceClass {  // インターフェースクラス
public:
    virtual void DoSomething(int32_t) = 0;
    virtual bool IsXxx() const        = 0;
    virtual ~InterfaceClass()         = 0;
};

class NotInterfaceClass {  // メンバ変数があるためインターフェースクラスではない
public:
    NotInterfaceClass();
    virtual void DoSomething(int32_t) = 0;
    virtual bool IsXxx() const        = 0;
    virtual ~NotInterfaceClass()      = 0;

private:
    int32_t num_;
};
// @@@ sample end
SUPPRESS_WARN_END;
}  // namespace
