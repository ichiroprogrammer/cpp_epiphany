#include "gtest_wrapper.h"

#include "string_holder_new.h"
#include "string_holder_old.h"

namespace {
TEST(Pimpl, string_holder) { StringHolderNew inst; }
}  // namespace
