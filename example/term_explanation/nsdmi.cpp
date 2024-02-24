#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_UNUSED_PRIVATE_FIELD;
// @@@ sample begin 0:0

class A {
public:
    A() : a_{1}  // NSDMIではなく、非静的なメンバ初期化子による初期化
    {
    }

private:
    int32_t     a_;
    int32_t     b_ = 0;        // NSDMI
    std::string str_{"init"};  // NSDMI
};
// @@@ sample end
SUPPRESS_WARN_END;
}  // namespace
