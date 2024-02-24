#include <cstdint>
#include <iostream>

#include "gtest_wrapper.h"

namespace {
// @@@ sample begin 0:0

class Singleton final {
public:
    static Singleton&       Inst();
    static Singleton const& InstConst() noexcept  // constインスタンスを返す
    {
        return Inst();
    }
    // @@@ ignore begin

    void SetXxx(uint32_t xxx) noexcept { xxx_ = xxx; }

    uint32_t GetXxx() const noexcept { return xxx_; }
    // @@@ ignore end

private:
    Singleton() noexcept {}  // コンストラクタをprivateにすることで、
                             // Inst()以外ではこのオブジェクトを生成できない。
    // @@@ ignore begin

    uint32_t xxx_{0};
    // @@@ ignore end
};

Singleton& Singleton::Inst()
{
    static Singleton inst;  //  instの初期化が同時に行われることはない。

    return inst;
}

TEST(Singleton, how_to_use)
{
    auto&       inst       = Singleton::Inst();
    auto const& inst_const = Singleton::InstConst();

    ASSERT_EQ(0, inst.GetXxx());
    ASSERT_EQ(0, inst_const.GetXxx());
#if 0
    inst_const.SetXxx(10);  // inst_constはconstオブジェクトなのでコンパイルエラー
#else
    inst.SetXxx(10);
#endif
    ASSERT_EQ(10, inst.GetXxx());
    ASSERT_EQ(10, inst_const.GetXxx());

    inst.SetXxx(0);
    ASSERT_EQ(0, inst.GetXxx());
    ASSERT_EQ(0, inst_const.GetXxx());
}
}  // namespace
// @@@ sample end
