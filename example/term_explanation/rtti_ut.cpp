#include <exception>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

// @@@ sample begin 0:0

class Polymorphic_Base {  // ポリモーフィックな基底クラス
public:
    virtual ~Polymorphic_Base() = default;
};

class Polymorphic_Derived : public Polymorphic_Base {  // ポリモーフィックな派生クラス
};
// @@@ sample end

namespace {
TEST(RTTI, polymorphic)
{
    // @@@ sample begin 0:1

    auto b = Polymorphic_Base{};
    auto d = Polymorphic_Derived{};

    Polymorphic_Base& b_ref_d = d;
    Polymorphic_Base& b_ref_b = b;

    // std::type_infoの比較
    ASSERT_EQ(typeid(b_ref_d), typeid(d));
    ASSERT_EQ(typeid(b_ref_b), typeid(b));

    // ポインタへのdynamic_cast
    auto* d_ptr = dynamic_cast<Polymorphic_Derived*>(&b_ref_d);
    ASSERT_EQ(d_ptr, &d);

    auto* d_ptr2 = dynamic_cast<Polymorphic_Derived*>(&b_ref_b);
    ASSERT_EQ(d_ptr2, nullptr);  // キャストできない場合、nullptrが返る

    // リファレンスへのdynamic_cast
    auto& d_ref = dynamic_cast<Polymorphic_Derived&>(b_ref_d);
    ASSERT_EQ(&d_ref, &d);

    SUPPRESS_WARN_BEGIN;       // @@@ delete
    SUPPRESS_WARN_UNUSED_VAL;  // @@@ delete
    // キャストできない場合、エクセプションのが発生する
    ASSERT_THROW(dynamic_cast<Polymorphic_Derived&>(b_ref_b), std::bad_cast);
    // @@@ sample end
    SUPPRESS_WARN_END;
}
}  // namespace

// @@@ sample begin 1:0

class NonPolymorphic_Base {  // 非ポリモーフィックな基底クラス
};

class NonPolymorphic_Derived : public NonPolymorphic_Base {  // 非ポリモーフィックな派生クラス
};
// @@@ sample end

namespace {
TEST(RTTI, non_polymorphic)
{
    // @@@ sample begin 1:1

    auto b = NonPolymorphic_Base{};
    auto d = NonPolymorphic_Derived{};

    NonPolymorphic_Base& b_ref_d = d;
    NonPolymorphic_Base& b_ref_b = b;

    // std::type_infoの比較
    ASSERT_EQ(typeid(b_ref_d), typeid(b));  // 実際の型ではなく、表層型のtype_infoが返る
    ASSERT_EQ(typeid(b_ref_b), typeid(b));

    // virtual関数を持たないため、ポインタへのdynamic_castはコンパイルできない
    // auto* d_ptr = dynamic_cast<NonPolymorphic_Derived*>(&b_ref_d);
    // auto* d_ptr2 = dynamic_cast<NonPolymorphic_Derived*>(&b_ref_b);

    // virtual関数を持たないため、リファレンスへのdynamic_castはコンパイルできない
    // auto& d_ref = dynamic_cast<NonPolymorphic_Derived&>(b_ref_d);
    // ASSERT_THROW(dynamic_cast<NonPolymorphic_Derived&>(b_ref_b), std::bad_cast);
    // @@@ sample end
}
}  // namespace
