#include <cstdint>
#include <memory>

#include "gtest_wrapper.h"

namespace {
// @@@ sample begin 0:0

class A final {
public:
    explicit A(int32_t n) noexcept : num_{n} { last_constructed_num_ = num_; }
    ~A() { last_destructed_num_ = num_; }

    int32_t GetNum() const noexcept { return num_; }

    static int32_t LastConstructedNum() noexcept { return last_constructed_num_; }
    static int32_t LastDestructedNum() noexcept { return last_destructed_num_; }

private:
    int32_t const  num_;
    static int32_t last_constructed_num_;
    static int32_t last_destructed_num_;
};

int32_t A::last_constructed_num_ = -1;
int32_t A::last_destructed_num_  = -1;

class X final {
public:
    // Xオブジェクトの生成と、ptrからptr_へ所有権の移動
    explicit X(std::unique_ptr<A>&& ptr) : ptr_{std::move(ptr)} {}

    // ptrからptr_へ所有権の移動
    void Move(std::unique_ptr<A>&& ptr) noexcept { ptr_ = std::move(ptr); }

    // ptr_から外部への所有権の移動
    std::unique_ptr<A> Release() noexcept { return std::move(ptr_); }

    A const* GetA() const noexcept { return ptr_ ? ptr_.get() : nullptr; }

private:
    std::unique_ptr<A> ptr_{};
};
// @@@ sample end

TEST(UniqueOwnership, move)
{
    // @@@ sample begin 0:1

    // clang-format off
    // ステップ0
    // まだ、クラスAオブジェクトは生成されていないため、
    // A::LastConstructedNum()、A::LastDestructedNum()は初期値である-1である。
    ASSERT_EQ(-1, A::LastConstructedNum());     // まだ、A::A()は呼ばれてない
    ASSERT_EQ(-1, A::LastDestructedNum());      // まだ、A::~A()は呼ばれてない
    // @@@ sample end
    // @@@ sample begin 0:2

    // ステップ1
    // a0、a1がそれぞれ初期化される。
    auto a0 = std::make_unique<A>(0);           // a0はA{0}を所有
    auto a1 = std::make_unique<A>(1);           // a1はA{1}を所有

    ASSERT_EQ(1,  A::LastConstructedNum());     // A{1}は生成された
    ASSERT_EQ(-1, A::LastDestructedNum());      // まだ、A::~A()は呼ばれてない
    // @@@ sample end
    // @@@ sample begin 0:3

    // ステップ2
    // xが生成され、オブジェクトA{0}の所有がa0からxへ移動する。
    ASSERT_EQ(0, a0->GetNum());                 // a0はA{0}を所有
    auto x = X{std::move(a0)};                  // xの生成と、a0からxへA{0}の所有権の移動
    ASSERT_FALSE(a0);                           // a0は何も所有していない
    // @@@ sample end
    // @@@ sample begin 0:4

    // ステップ3
    // オブジェクトA{1}の所有がa1からxへ移動する。
    // xは以前保持していたA{0}オブジェクトへのポインタをdeleteするため
    // (std::unique_ptrによる自動delete)、A::LastDestructedNum()の値が0になる。
    ASSERT_EQ(1, a1->GetNum());                 // a1はA{1}を所有
    x.Move(std::move(a1));                      // xによるA{0}の解放
                                                // a1からxへA{1}の所有権の移動
    ASSERT_EQ(0, A::LastDestructedNum());       // A{0}は解放された
    ASSERT_FALSE(a1);                           // a1は何も所有していない
    ASSERT_EQ(1, x.GetA()->GetNum());           // xはA{1}を所有
    // @@@ sample end
    // @@@ sample begin 0:5

    // ステップ4
    // x.ptr_はstd::unique_ptr<A>であるため、ステップ3の状態では、
    // x.ptr_はA{1}オブジェクトのポインタを保持しているが、
    // x.Release()はそれをrvalueに変換し戻り値にする。
    // その戻り値をa2で受け取るため、A{1}の所有はxからa2に移動する。
    std::unique_ptr<A> a2{x.Release()};         // xからa2へA{1}の所有権の移動
    ASSERT_EQ(nullptr, x.GetA());               // xは何も所有していない
    ASSERT_EQ(1, a2->GetNum());                 // a2はA{1}を所有
    // @@@ sample end
    // @@@ sample begin 0:6

    // ステップ5
    // a2をstd::move()によりrvalueに変換し、ブロック内のa3に渡すことで、
    // A{1}の所有はa2からa3に移動する。
    {
        std::unique_ptr<A> a3{std::move(a2)};
        ASSERT_FALSE(a2);                       // a2は何も所有していない
        ASSERT_EQ(1, a3->GetNum());             // a3はA{1}を所有
        // @@@ sample end
        // @@@ sample begin 0:7

        // ステップ6
        // このブロックが終了することで、std::unique_ptrであるa3のデストラクタが呼び出される。
        // これはA{1}オブジェクトへのポインタをdeleteする。
    }                                      // a3によるA{1}の解放
    ASSERT_EQ(1, A::LastDestructedNum());  // A{1}が解放されたことの確認
    // @@@ sample end
    // clang-format on
}

TEST(UniqueOwnership, copy)
{
    // clang-format off
    // @@@ sample begin 1:0

    auto a0 = std::make_unique<A>(0);

    // auto a1 = a0;                            // 下記のようなメッセージでコンパイルエラー
    //      unique_ptr_ownership_ut.cpp:125:15: error: use of deleted function ‘std::unique_ptr ...

    auto a1 = std::move(a0);                    // すでに示したようにmove生成は可能
    
    auto a2 = std::unique_ptr<A>{};

    // a2 = a1;                                 // 下記のようなメッセージでコンパイルエラー
    //      unique_ptr_ownership_ut.cpp:131:10: error: use of deleted function ‘std::unique_ptr ...

    a2 = std::move(a1);                         // すでに示したようにmove代入は可能

    //
    auto x0 = X{std::make_unique<A>(0)};

    // auto x1 = x0;                            // Xはstd::unique_ptrをメンバとするため、
                                                // デフォルトのcopyコンストラクタによる生成は
                                                // コンパイルエラー
    auto x1 = std::move(x0);                    // デフォルトのmove生成は可能

    auto x2 = X{std::make_unique<A>(0)};

    // x2 = x1;                                 // Xはstd::unique_ptrをメンバとするため、
                                                // デフォルトのcopy代入子の呼び出しは
                                                // コンパイルエラー
    x2 = std::move(x1);                         // デフォルトのmove代入は可能
    // @@@ sample end

    // clang-format on
}

TEST(UniqueOwnership, DISABLED_invalid)
{
#ifndef __clang_analyzer__
    // @@@ sample begin 2:0

    // 以下のようなコードを書いてはならない

    auto a0 = std::make_unique<A>(0);
    auto a1 = std::unique_ptr<A>{a0.get()};  // a1もa0が保持するオブジェクトを保持するが、
                                             // 保持されたオブジェクトは二重解放される

    auto a_ptr = new A{0};

    auto a2 = std::unique_ptr<A>{a_ptr};
    auto a3 = std::unique_ptr<A>{a_ptr};  // a3もa2が保持するオブジェクトを保持するが、
                                          // 保持されたオブジェクトは二重解放される
    // @@@ sample end
#endif
}
}  // namespace
