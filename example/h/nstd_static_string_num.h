#pragma once

#include <utility>

#include "nstd_static_string.h"

namespace Nstd {
// @@@ sample begin 0:0

namespace Inner_ {

// 10進数桁数を返す
constexpr size_t num_of_digits(size_t n) noexcept { return n > 0 ? 1 + num_of_digits(n / 10) : 0; }

// 10のn乗を返す
constexpr uint32_t ten_to_nth_power(uint32_t n) noexcept
{
    return n == 0 ? 1 : 10 * ten_to_nth_power(n - 1);
}

// 10進数の桁の若い順番に左から並べなおす(12345 -> 54321)
constexpr uint32_t reverse_num(uint32_t num) noexcept
{
    return num != 0 ? (num % 10) * ten_to_nth_power(num_of_digits(num) - 1) + reverse_num(num / 10)
                    : 0;
}

// 10進数一桁をacsii文字に変換
constexpr char digit_to_char(uint32_t num, uint32_t n_th) noexcept
{
    return '0' + (num % (ten_to_nth_power(n_th + 1))) / ten_to_nth_power(n_th);
}

// Int2StaticStringのヘルパ関数
template <size_t N, size_t... Cs>
constexpr StaticString<num_of_digits(N) + 1> make_static_string(std::index_sequence<Cs...>) noexcept
{
    return {digit_to_char(reverse_num(N), Cs)...};
}
}  // namespace Inner_
// @@@ sample end
// @@@ sample begin 0:1

template <size_t N>
constexpr StaticString<Inner_::num_of_digits(N) + 1> Int2StaticString() noexcept
{
    return Inner_::make_static_string<N>(std::make_index_sequence<Inner_::num_of_digits(N)>());
}
// @@@ sample end
}  // namespace Nstd
