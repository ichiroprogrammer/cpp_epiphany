#include "gtest_wrapper.h"

#include "enum_operator.h"
#include "suppress_warning.h"

namespace NotUseBitmaskType {
// ASSERT_EQがSwimのリファレンスにマッチするので、linkには実態が必要。
constexpr uint32_t Animal::PhisicalAbility::Swim;

namespace {
TEST(BitmaskType, not_use)
{
    // @@@ sample begin 0:0

    Animal dolphin{Animal::PhisicalAbility::Swim};  // OK
    ASSERT_EQ(Animal::PhisicalAbility::Swim, dolphin.GetPhisicalAbility());

    Animal uma{0xff};  // NG 誤用だが、コンストラクタの仮引数の型がuint32_tなのでコンパイル可能
    // @@@ sample end
}
}  // namespace
}  // namespace NotUseBitmaskType

namespace UseBitmaskType {
namespace {
TEST(BitmaskType, use)
{
    // @@@ sample begin 1:0

    // コンストラクタの仮引数の型が厳密になったためコンパイル不可
    // これにより誤用を防ぐ
    // Animal uma{0xff};

    // C++17から下記はコンパイル可能となったが、アクシデントでこのようなミスはしないだろう
    auto uma = Animal{Animal::PhisicalAbility{0xff}};
    IGNORE_UNUSED_VAR(uma);  // @@@ delete

    auto dolphin = Animal{Animal::PhisicalAbility::Swim};
    ASSERT_EQ(Animal::PhisicalAbility::Swim, dolphin.GetPhisicalAbility());

    auto pa = Animal::PhisicalAbility{Animal::PhisicalAbility::Run};
    pa |= Animal::PhisicalAbility::Swim;

    auto human = Animal{pa};
    ASSERT_TRUE(IsTrue(Animal::PhisicalAbility::Run & human.GetPhisicalAbility()));
    // @@@ sample end

    auto swan = Animal{Animal::PhisicalAbility::Run | Animal::PhisicalAbility::Fly
                       | Animal::PhisicalAbility::Swim};
    ASSERT_TRUE(IsTrue(Animal::PhisicalAbility::Run & swan.GetPhisicalAbility()));
    ASSERT_TRUE(IsTrue(Animal::PhisicalAbility::Fly & swan.GetPhisicalAbility()));
    ASSERT_TRUE(!IsFalse(Animal::PhisicalAbility::Swim & swan.GetPhisicalAbility()));

    auto pa2 = Animal::PhisicalAbility{Animal::PhisicalAbility::Swim};
    pa2 |= Animal::PhisicalAbility::Run;
    pa2 |= Animal::PhisicalAbility::Fly;
    pa2 &= Animal::PhisicalAbility::Run | Animal::PhisicalAbility::Fly;

    auto bat = Animal{pa2};

    ASSERT_TRUE(IsTrue(Animal::PhisicalAbility::Run & bat.GetPhisicalAbility()));
    ASSERT_TRUE(IsTrue(Animal::PhisicalAbility::Fly & bat.GetPhisicalAbility()));
    ASSERT_TRUE(IsFalse(Animal::PhisicalAbility::Swim & bat.GetPhisicalAbility()));
}
}  // namespace
}  // namespace UseBitmaskType
