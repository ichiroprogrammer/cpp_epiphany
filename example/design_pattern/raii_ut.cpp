#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>

#include "gtest_wrapper.h"

#include "scoped_guard.h"

namespace {

class A* a_ptr;

// @@@ sample begin 0:0

// Aは外部の変数をリファレンスcounter_として保持し、
//  * コンストラクタ呼び出し時に++counter_
//  * デストラクタタ呼び出し時に--counter_
// とするため、生成と解放が同じだけ行われれば外部の変数の値は0となる
class A {
public:
    A(uint32_t& couner) noexcept : counter_{++couner} {}
    ~A() { --counter_; }

private:
    uint32_t& counter_;
};

char not_use_RAII_for_memory(size_t index, uint32_t& object_counter)
{
    auto a = new A{object_counter};  // RAIIでない例
    auto s = std::string{"hehe"};
    a_ptr  = a;  // @@@ delete LeakSanitizer対策

    auto ret = s.at(index);  // index >= 5でエクセプション発生

    // 何らかの処理

    delete a;         //  この行以前に関数を抜けるとaはメモリリーク
    a_ptr = nullptr;  // @@@ delete LeakSanitizer対策

    return ret;
}
// @@@ sample end

class RAII : public ::testing::Test {
protected:
    virtual void SetUp() noexcept override {}

    virtual void TearDown() noexcept override
    {
        delete a_ptr;  // @@@ delete LeakSanitizer対策
        a_ptr = nullptr;
    }
};

class DISABLED_RAII : public RAII {};

#if defined(__clang__)  // clang++ではクラッシュしてしまう
#define RAII_Opt DISABLED_RAII
#else
#define RAII_Opt RAII
#endif

TEST_F(RAII_Opt, leak)
{
    // @@@ sample begin 0:1

    auto object_counter = 0U;

    // 第1引数が5なのでエクセプション発生
    ASSERT_THROW(not_use_RAII_for_memory(5, object_counter), std::exception);

    // 上記のnot_use_RAII_for_memoryではエクセプションが発生し、メモリリークする
    ASSERT_EQ(1, object_counter);
    // @@@ sample end
}

// @@@ sample begin 1:0

char use_RAII_for_memory(size_t index, uint32_t& object_counter)
{
    auto a = std::make_unique<A>(object_counter);
    auto s = std::string{"hehe"};

    auto ret = s.at(index);  // index >= 5でエクセプション発生

    // 何らかの処理

    return ret;  // aは自動解放される
}
// @@@ sample end

TEST_F(RAII, not_leak)
{
    // @@@ sample begin 1:1

    auto object_counter = 0U;

    // 第1引数が5なのでエクセプション発生
    ASSERT_THROW(use_RAII_for_memory(5, object_counter), std::exception);

    // 上記のuse_RAII_for_memoryではエクセプションが発生するがメモリリークはしない
    ASSERT_EQ(0, object_counter);
    // @@@ sample end
}
// @@@ sample begin 2:0

// RAIIをしない例
// 複数のclose()を書くような関数は、リソースリークを起こしやすい。
void not_use_RAII_for_socket()
{
    auto fd = socket(AF_INET, SOCK_STREAM, 0);

    try {
        // Do something
        // @@@ ignore begin
        // @@@ ignore end
    }
    catch (std::exception const& e) {  // エクセプションはconstリファレンスで受ける。
        close(fd);                     // NG RAII未使用
        // Do something to recover
        // @@@ ignore begin
        std::cout << e.what() << std::endl;
        // @@@ ignore end

        return;
    }
    // @@@ ignore begin
    // @@@ ignore end
    close(fd);  // NG RAII未使用
}
// @@@ sample end
// @@@ sample begin 2:1

// RAIIをScopedGuardで行った例。
// close()が自動実行されるためにリソース解放を忘れない。
void use_RAII_for_socket()
{
    auto fd    = socket(AF_INET, SOCK_STREAM, 0);
    auto guard = ScopedGuard{[fd] { close(fd); }};  // 関数終了時に自動実行

    try {
        // Do something
    }
    catch (...) {
        // Do something to recover

        return;
    }
    // @@@ ignore begin
    // @@@ ignore end
}
// @@@ sample end

TEST_F(RAII, scoped_guard)
{
    not_use_RAII_for_socket();
    use_RAII_for_socket();
}
}  // namespace
