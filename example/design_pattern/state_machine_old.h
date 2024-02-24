#pragma once
#include <string_view>

// @@@ sample begin 0:0

extern std::string_view ThreadOldStyleStateStr() noexcept;
extern void             ThreadOldStyleRun();
extern void             ThreadOldStyleAbort();
extern void             ThreadOldStyleSuspend();
// @@@ sample end
