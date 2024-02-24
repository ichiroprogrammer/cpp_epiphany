#include <stdexcept>

#include "suppress_warning.h"

namespace {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_UNUSED_FUNC;
// @@@ sample begin 0:0

void function_try_block()
try {  // 関数tryブロック
    // 何らかの処理
    // @@@ ignore begin
    // @@@ ignore end
}
catch (std::length_error const& e) {  // 関数tryブロックのエクセプションハンドラ
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(e);
    // @@@ ignore end
}
catch (std::logic_error const& e) {  // 関数tryブロックのエクセプションハンドラ
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(e);
    // @@@ ignore end
}
// @@@ sample end
SUPPRESS_WARN_END;
}  // namespace
