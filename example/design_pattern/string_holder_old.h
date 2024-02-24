#pragma once

// @@@ sample begin 0:0
// このファイルには<string>が必要

#include <memory>
#include <string>

class StringHolderOld final {
public:
    StringHolderOld();

    void        Add(char const* str);
    char const* GetStr() const;

private:
    std::unique_ptr<std::string> str_;
};
// @@@ sample end
