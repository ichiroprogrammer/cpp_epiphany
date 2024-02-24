// @@@ sample begin 0:0
// このファイルには<string>が必要

#include <string>

#include "string_holder_new.h"

class StringHolderNew::StringHolderNewCore final {
public:
    StringHolderNewCore() = default;
    void Add(char const* str) { str_ += str; }

    char const* GetStr() const noexcept { return str_.c_str(); }

private:
    std::string str_{};
};

StringHolderNew::StringHolderNew() : core_{std::make_unique<StringHolderNewCore>()} {}

void StringHolderNew::Add(char const* str) { core_->Add(str); }

char const* StringHolderNew::GetStr() const { return core_->GetStr(); }

// この宣言、定義をしないと、StringHolderNewをインスタンス化した場所では、
// StringHolderNewCoreが不完全型であるため、std::unique_ptrが実体化できず、コンパイルエラーとなる。
// この場所であれば、StringHolderNewCoreは完全型であるためstd::unique_ptrが実体化できる。
StringHolderNew::~StringHolderNew() = default;
// @@@ sample end
