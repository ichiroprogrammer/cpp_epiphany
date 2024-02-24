#include "gtest_wrapper.h"

#include "mpool_allocator.h"
#include "mpool_variable.h"
#include "op_new.h"

// @@@ sample begin 0:0

namespace {
MPoolVariable<1024 * 64> mpv_allocator;
}

template <>
MPool& MPoolBasedAllocator<char>::mpool_ = mpv_allocator;
// @@@ sample end

namespace {
TEST(NewDelete, string_with_allocator)
{
    // @@@ sample begin 0:1

    auto rest = mpv_allocator.GetCount();
    auto str  = std::basic_string<char, std::char_traits<char>, MPoolBasedAllocator<char>>{"hehe"};

    ASSERT_TRUE(mpv_allocator.IsValid(str.c_str()));  // mpv_allocatorを使用してメモリ確保
    ASSERT_GT(rest, mpv_allocator.GetCount());  // mpv_allocatorのメモリが減っていることの確認
    // @@@ sample end
}
}  // namespace
// @@@ sample begin 0:2

using mpv_string = std::basic_string<char, std::char_traits<char>, MPoolBasedAllocator<char>>;
// @@@ sample end

namespace {
TEST(NewDelete, string_with_allocator2)
{
    auto rest = mpv_allocator.GetCount();
    auto str  = std::basic_string<char, std::char_traits<char>, MPoolBasedAllocator<char>>{"hehe"};

    ASSERT_TRUE(mpv_allocator.IsValid(str.c_str()));  // mpv_allocatorのメモリであることの確認
    ASSERT_GT(rest, mpv_allocator.GetCount());  // mpv_allocatorのメモリが減っていることの確認
}
}  // namespace

// @@@ sample begin 1:0

template <>
MPool& MPoolBasedAllocator<int>::mpool_ = mpv_allocator;

using mpv_vector_int = std::vector<int, MPoolBasedAllocator<int>>;
// @@@ sample end

namespace {
TEST(NewDelete, vector_int_with_allocator)
{
    // @@@ sample begin 1:1

    auto rest = mpv_allocator.GetCount();
    auto ints = mpv_vector_int{1, 2, 3};

    ASSERT_TRUE(mpv_allocator.IsValid(&ints[0]));  // mpv_allocatorのメモリであることの確認
    ASSERT_GT(rest, mpv_allocator.GetCount());  // mpv_allocatorのメモリが減っていることの確認
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 2:0

using mpv_vector_str = std::vector<mpv_string, MPoolBasedAllocator<mpv_string>>;

template <>
MPool& MPoolBasedAllocator<mpv_string>::mpool_ = mpv_allocator;
// @@@ sample end

namespace {
TEST(NewDelete, allocator)
{
    auto rest = mpv_allocator.GetCount();

    auto ints = mpv_vector_int{1, 2, 3};

    ASSERT_GT(rest, mpv_allocator.GetCount());
    rest = mpv_allocator.GetCount();

    for (auto const& i : ints) {
        ASSERT_TRUE(mpv_allocator.IsValid(&i));  // mpv_allocatorのメモリであることの確認
    }

    // @@@ sample begin 2:1

    auto strs = mpv_vector_str{"1", "2", "3"};

    ASSERT_GT(rest, mpv_allocator.GetCount());

    for (auto const& s : strs) {
        ASSERT_TRUE(mpv_allocator.IsValid(&s));  // mpv_allocatorのメモリであることの確認
        ASSERT_TRUE(mpv_allocator.IsValid(s.c_str()));  // mpv_allocatorのメモリであることの確認
    }
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 3:0

struct mpv_string_op_new : OpNew<mpv_string_op_new>, mpv_string {
    using mpv_string::basic_string;
};

template <>
MPool& OpNew<mpv_string_op_new>::mpool_ = mpv_allocator;
// @@@ sample end

namespace {
TEST(NewDelete, allocator2)
{
    // @@@ sample begin 3:1

    auto rest = mpv_allocator.GetCount();

    auto str0 = std::make_unique<mpv_string>();  // グローバルnewが使われる

    // mpv_stringのnewにはmpv_allocatorは使われない
    ASSERT_FALSE(mpv_allocator.IsValid(str0.get()));
    ASSERT_EQ(rest, mpv_allocator.GetCount());
    // @@@ sample end

    {
        // @@@ sample begin 3:2

        rest = mpv_allocator.GetCount();

        auto str1 = std::make_unique<mpv_string_op_new>();

        // mpv_string_op_newのnewにmpv_allocatorが使われる
        ASSERT_TRUE(mpv_allocator.IsValid(str1.get()));
        ASSERT_GT(rest, mpv_allocator.GetCount());
        // @@@ sample end
    }
    {
        // @@@ sample begin 3:3

        rest = mpv_allocator.GetCount();

        auto str2 = std::make_shared<mpv_string_op_new>();

        // mpv_string_op_newのnewにmpv_allocatorが使われない!!!
        ASSERT_FALSE(mpv_allocator.IsValid(str2.get()));
        ASSERT_EQ(rest, mpv_allocator.GetCount());
        // @@@ sample end
    }
    {
        // @@@ sample begin 3:4

        rest = mpv_allocator.GetCount();

        auto str3 = std::shared_ptr<mpv_string_op_new>{new mpv_string_op_new};

        // mpv_string_op_newのnewにmpv_allocatorが使われる
        ASSERT_TRUE(mpv_allocator.IsValid(str3.get()));
        ASSERT_GT(rest, mpv_allocator.GetCount());
        // @@@ sample end
    }
    {
        rest      = mpv_allocator.GetCount();
        auto str4 = std::make_unique<mpv_string_op_new>("hehe");

        // mpv_string_op_newのnewにmpv_allocatorが使われる
        ASSERT_TRUE(mpv_allocator.IsValid(str4.get()));

        // mpv_string_op_newの内部メモリ確保にmpv_allocatorが使われる
        ASSERT_TRUE(mpv_allocator.IsValid(str4->c_str()));
        ASSERT_GT(rest, mpv_allocator.GetCount());
    }
}

TEST(NewDelete, allocator_itor)
{
    constexpr auto vec_len = 10U;

    std::cout << "mpv_allocator:" << __LINE__ << std::endl;
    for (auto itor = mpv_allocator.cbegin(); itor != mpv_allocator.cend(); ++itor) {
        std::cout << std::setw(16) << (*itor)->next << ":" << (*itor)->n_nuits << std::endl;
    }

    {
        auto strs = mpv_vector_str{};

        {
            auto strs2 = mpv_vector_str{};

            std::cout << "mpv_allocator:" << __LINE__ << std::endl;
            for (auto itor = mpv_allocator.cbegin(); itor != mpv_allocator.cend(); ++itor) {
                std::cout << std::setw(16) << (*itor)->next << ":" << (*itor)->n_nuits << std::endl;
            }

            for (auto i = 0U; i < vec_len; ++i) {
                strs2.emplace_back(i, 'a');
                strs.emplace_back(i, 'b');
            }

            std::cout << "mpv_allocator:" << __LINE__ << std::endl;
            for (auto itor = mpv_allocator.cbegin(); itor != mpv_allocator.cend(); ++itor) {
                std::cout << std::setw(16) << (*itor)->next << ":" << (*itor)->n_nuits << std::endl;
            }
        }

        std::cout << "mpv_allocator:" << __LINE__ << std::endl;
        // @@@ sample begin 4:0

        for (auto mem : mpv_allocator) {
            std::cout << std::setw(16) << mem->next << ":" << mem->n_nuits << std::endl;
        }
        // @@@ sample end
    }

    std::cout << "mpv_allocator:" << __LINE__ << std::endl;
    for (auto itor = mpv_allocator.cbegin(); itor != mpv_allocator.cend(); ++itor) {
        std::cout << std::setw(16) << (*itor)->next << ":" << (*itor)->n_nuits << std::endl;
    }
}
}  // namespace
