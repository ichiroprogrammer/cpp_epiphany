#pragma once

// @@@ sample begin 0:0
// このファイルには<string>は不要

#include <memory>

class StringHolderNew final {
public:
    StringHolderNew();

    void        Add(char const* str);
    char const* GetStr() const;
    ~StringHolderNew();  // デストラクタは.cppで=defaultで定義

private:
    class StringHolderNewCore;  // StringHolderNewの振る舞いは、StringHolderNewCoreに移譲
    std::unique_ptr<StringHolderNewCore> core_;
};
// @@@ sample end
