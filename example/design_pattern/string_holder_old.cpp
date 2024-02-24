// @@@ sample begin 0:0

#include "string_holder_old.h"

StringHolderOld::StringHolderOld() : str_{std::make_unique<std::string>()} {}

void StringHolderOld::Add(char const* str) { *str_ += str; }

char const* StringHolderOld::GetStr() const { return str_->c_str(); }
// @@@ sample end
