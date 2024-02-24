#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

// @@@ sample begin 0:0

class ShallowOK {
public:
    explicit ShallowOK(char const* str = "") : str_{std::string{str}} {}
    std::string const& GetString() const noexcept { return str_; }

    // 下記2関数を定義しなければ、以下と同等なcopyコンストラクタ、copy代入演算子が定義される。
    ShallowOK(ShallowOK const& rhs) : str_{rhs.str_} {}

    ShallowOK& operator=(ShallowOK const& rhs)
    {
        str_ = rhs.str_;
        return *this;
    }

private:
    std::string str_;
};
// @@@ sample end

TEST(DeepShallowCopy, shallow_ok)
{
    auto const s0 = ShallowOK{"s0"};
    auto const s1 = ShallowOK{s0};
    auto       s2 = ShallowOK{"s2"};

    s2 = s0;

    ASSERT_EQ("s0", s0.GetString());
    ASSERT_EQ("s0", s1.GetString());
    ASSERT_EQ("s0", s2.GetString());
}

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_GCC_NOT_EFF_CPP;
// @@@ sample begin 1:0

class ShallowNG {
public:
    explicit ShallowNG(char const* str = "") : str_{new std::string{str}} {}
    ~ShallowNG() { delete str_; }
    std::string const& GetString() const noexcept { return *str_; }

private:
    std::string* str_;
};
// @@@ sample end
SUPPRESS_WARN_END;

#define DOUBLE_DELETE 0
TEST(DeepShallowCopy, shallow_ng)
{
    // @@@ sample begin 1:1

    auto const s0 = ShallowNG{"s0"};
#if DOUBLE_DELETE == 1  // @@@ delete

    // NG s0.str_とs1.str_は同じメモリを指すため~ShallowNG()に2重解放される。
    auto const s1 = ShallowNG{s0};

    auto s2 = ShallowNG{"s2"};

    // NG s2.str_が元々保持していたメモリは、解放できなくなる。
    s2 = s0;

    // NG s0.str_とs2.str_は同じメモリを指すため、
    //    s0、s2のスコープアウト時に、~ShallowNG()により、2重解放される。
    // @@@ sample end
#endif
}

// @@@ sample begin 2:0

class Deep {
public:
    explicit Deep(char const* str = "") : str_{new std::string{str}} {}
    ~Deep() { delete str_; }
    std::string const& GetString() const noexcept { return *str_; }

    // copyコンストラクタの実装例
    Deep(Deep const& rhs) : str_{new std::string{*rhs.str_}} {}

    // copy代入演算子の実装例
    Deep& operator=(Deep const& rhs)
    {
        *str_ = *(rhs.str_);
        return *this;
    }

private:
    std::string* str_;
};

class Deep2 {  // std::unique_ptrを使いDeepをリファクタリング
public:
    explicit Deep2(char const* str = "") : str_{std::make_unique<std::string>(str)} {}
    std::string const& GetString() const { return *str_; }

    // copyコンストラクタの実装例
    Deep2(Deep2 const& rhs) : str_{std::make_unique<std::string>(*rhs.str_)} {}

    // copy代入演算子の実装例
    Deep2& operator=(Deep2 const& rhs)
    {
        *str_ = *(rhs.str_);
        return *this;
    }

private:
    std::unique_ptr<std::string> str_;
};
// @@@ sample end

TEST(DeepShallowCopy, deep_ok)
{
    auto const d0 = Deep{"d0"};
    auto const d1 = Deep{d0};
    auto       d2 = Deep{"d2"};

    d2 = d0;  // OK d0::str_とd1::str_は別のメモリを指す。

    ASSERT_EQ("d0", d0.GetString());
    ASSERT_EQ("d0", d1.GetString());
    ASSERT_EQ("d0", d2.GetString());
}

TEST(DeepShallowCopy, deep2_ok)
{
    auto const d0 = Deep2{"d0"};
    auto const d1 = Deep2{d0};
    auto       d2 = Deep2{"d2"};

    d2 = d0;  // OK d0::str_とd1::str_は別のメモリを指す。

    ASSERT_EQ("d0", d0.GetString());
    ASSERT_EQ("d0", d1.GetString());
    ASSERT_EQ("d0", d2.GetString());
}
}  // namespace
