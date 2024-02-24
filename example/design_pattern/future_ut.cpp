#include <future>
#include <string>
#include <thread>

#include "gtest_wrapper.h"

#include "do_heavy_algorithm.h"

namespace {

// @@@ sample begin 0:0

int do_something(std::string_view str0, std::string_view str1) noexcept
{
    // @@@ ignore begin
    auto ret0 = str0.size();
    auto ret1 = str1.size();
    // 何かの処理
    // @@@ ignore end

    return ret0 + ret1;
}

TEST(Future, old_style)
{
    auto str0 = std::string{};
    auto th0  = std::thread{[&str0]() noexcept { str0 = do_heavy_algorithm("thread 0"); }};

    auto str1 = std::string{};
    auto th1  = std::thread{[&str1]() noexcept { str1 = do_heavy_algorithm("thread 1"); }};

    //
    // このスレッドで行うべき何らかの処理
    //

    th0.join();
    th1.join();

    ASSERT_EQ("THREAD 0", str0);
    ASSERT_EQ("THREAD 1", str1);

    ASSERT_EQ(16, do_something(str0, str1));
}
// @@@ sample end
// @@@ sample begin 0:1

TEST(Future, new_style)
{
    std::future<std::string> result0
        = std::async(std::launch::async, []() noexcept { return do_heavy_algorithm("thread 0"); });

    std::future<std::string> result1
        = std::async(std::launch::async, []() noexcept { return do_heavy_algorithm("thread 1"); });

    // futre::get()は処理の待ち合わせと値の取り出しを行う。
    auto str0 = result0.get();
    auto str1 = result1.get();
    ASSERT_EQ(16, do_something(str0, str1));

    ASSERT_EQ("THREAD 0", str0);
    ASSERT_EQ("THREAD 1", str1);
}
// @@@ sample end
}  // namespace
