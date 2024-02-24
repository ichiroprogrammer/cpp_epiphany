#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

int32_t f() { return 1; }

// @@@ sample begin 0:0
int32_t f();
int32_t g1()
{
    if (auto ret = f(); ret != 0) {  // retがsimple-declaration
        return ret;
    }
    else {
        return 0;
    }
}

int32_t g2()
{
    switch (auto ret = f()) {  // retがsimple-declaration
    case 0:
        return 0;
    case 1:
        return ret * 5;
    case 2:
        return ret + 3;
    default:
        return -1;
    }
}
// @@@ sample end
TEST(TermExplanationMisra, simple_declaration)
{
    ASSERT_EQ(g1(), 1);
    ASSERT_EQ(g2(), 5);
}
}  // namespace
