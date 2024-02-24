#include <optional>

#include "gtest_wrapper.h"

namespace {

uint32_t next_prime_num(uint32_t curr_prime_num, std::vector<bool>& is_num_prime) noexcept
{
    for (auto i = 2 * curr_prime_num; i < is_num_prime.size(); i += curr_prime_num) {
        is_num_prime[i] = false;  // 次の倍数は素数ではない
    }

    auto prime_num = curr_prime_num;

    do {  // 次の素数の探索
        ++prime_num;
    } while (!is_num_prime[prime_num] && (prime_num < is_num_prime.size()));

    return prime_num;
}

namespace NotGuard {

// @@@ sample begin 0:0

/// @fn int32_t SequentialA(char const (&a)[3])
/// @brief a(配列へのリファレンス)の要素について、先頭から'a'が続く数を返す
/// @param 配列へのリファレンス
int32_t SequentialA(char const (&a)[3]) noexcept
{
    if (a[0] == 'a') {
        if (a[1] == 'a') {
            if (a[2] == 'a') {
                return 3;
            }
            else {
                return 2;
            }
        }
        else {
            return 1;
        }
    }
    else {
        return 0;
    }
}
// @@@ sample end
// @@@ sample begin 0:1

std::optional<std::vector<uint32_t>> PrimeNumbers(uint32_t max_num)
{
    auto result = std::vector<uint32_t>{};

    if (max_num < 65536) {  // 演算コストが高いためエラーにする
        if (max_num >= 2) {
            auto is_num_prime = std::vector<bool>(max_num + 1, true);  // falseなら素数でない
            is_num_prime[0] = is_num_prime[1] = false;
            auto prime_num                    = 2U;  // 最初の素数

            do {
                result.emplace_back(prime_num);
                prime_num = next_prime_num(prime_num, is_num_prime);
            } while (prime_num < is_num_prime.size());
        }

        return result;
    }

    return std::nullopt;
}
// @@@ sample end

}  // namespace NotGuard

namespace Guard {

// @@@ sample begin 1:0

int32_t SequentialA(char const (&a)[3]) noexcept
{
    if (a[0] != 'a') {  // ガード節
        return 0;
    }
    if (a[1] != 'a') {  // ガード節
        return 1;
    }
    if (a[2] != 'a') {  // ガード節
        return 2;
    }

    return 3;
}
// @@@ sample end
// @@@ sample begin 1:1

std::optional<std::vector<uint32_t>> PrimeNumbers(uint32_t max_num)
{
    if (max_num >= 65536) {  // ガード節。演算コストが高いためエラーにする。
        return std::nullopt;
    }

    auto result = std::vector<uint32_t>{};

    if (max_num < 2) {  // ガード節。2未満の素数はない。
        return result;
    }

    auto is_num_prime = std::vector<bool>(max_num + 1, true);  // falseなら素数でない。
    is_num_prime[0] = is_num_prime[1] = false;
    auto prime_num                    = 2U;  // 最初の素数

    do {
        result.emplace_back(prime_num);
        prime_num = next_prime_num(prime_num, is_num_prime);
    } while (prime_num < is_num_prime.size());

    return result;
}
// @@@ sample end

}  // namespace Guard
namespace NotGuard {
TEST(Guard, prime_number)
{
    auto result = PrimeNumbers(1);

    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{}), *result);

    result = PrimeNumbers(2);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2}), *result);

    result = PrimeNumbers(3);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3}), *result);

    result = PrimeNumbers(4);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3}), *result);

    result = PrimeNumbers(5);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3, 5}), *result);

    result = PrimeNumbers(6);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3, 5}), *result);

    result = PrimeNumbers(7);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3, 5, 7}), *result);

    result = PrimeNumbers(70000);
    ASSERT_FALSE(result);
}

TEST(Guard, SequentialA)
{
    ASSERT_EQ(0, SequentialA({'A', 'b', 'c'}));
    ASSERT_EQ(1, SequentialA({'a', 'b', 'c'}));
    ASSERT_EQ(2, SequentialA({'a', 'a', 'c'}));
    ASSERT_EQ(3, SequentialA({'a', 'a', 'a'}));
}
}  // namespace NotGuard

namespace Guard {

TEST(Guard, prime_number)
{
    auto result = PrimeNumbers(1);

    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{}), *result);

    result = PrimeNumbers(2);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2}), *result);

    result = PrimeNumbers(3);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3}), *result);

    result = PrimeNumbers(4);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3}), *result);

    result = PrimeNumbers(5);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3, 5}), *result);

    result = PrimeNumbers(6);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3, 5}), *result);

    result = PrimeNumbers(7);
    ASSERT_TRUE(result);
    ASSERT_EQ((std::vector<uint32_t>{2, 3, 5, 7}), *result);

    result = PrimeNumbers(70000);
    ASSERT_FALSE(result);
}

TEST(Guard, SequentialA)
{
    ASSERT_EQ(0, SequentialA({'A', 'b', 'c'}));
    ASSERT_EQ(1, SequentialA({'a', 'b', 'c'}));
    ASSERT_EQ(2, SequentialA({'a', 'a', 'c'}));
    ASSERT_EQ(3, SequentialA({'a', 'a', 'a'}));
}
}  // namespace Guard
}  // namespace
