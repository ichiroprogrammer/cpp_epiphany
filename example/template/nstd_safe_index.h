#pragma once
#include <array>
#include <string>
#include <vector>

#include "nstd_type_traits.h"

// @@@ sample begin 0:0

namespace Nstd {

template <template <class...> class C, typename... Ts>
struct SafeIndex : C<Ts...> {
    using C<Ts...>::C;

    using base_type = C<Ts...>;
    using size_type = typename base_type::size_type;

    typename base_type::reference       operator[](size_type i) { return this->at(i); }
    typename base_type::const_reference operator[](size_type i) const { return this->at(i); }
};
}  // namespace Nstd
// @@@ sample end
// @@@ sample begin 1:0

namespace Nstd {

template <typename T>
using SafeVector = Nstd::SafeIndex<std::vector, T>;

using SafeString = Nstd::SafeIndex<std::basic_string, char>;
}  // namespace Nstd
// @@@ sample end
// @@@ sample begin 2:0

namespace Nstd {
namespace Inner_ {

template <typename T, typename U>
struct std_array_like : std::array<T, U::value> {
    using std::array<T, U::value>::array;

    template <typename... ARGS>
    std_array_like(ARGS... args) noexcept(std::is_nothrow_constructible_v<T, ARGS...>)
        : std::array<T, U::value>{args...}
    {
        static_assert(AreConvertibleV<T, ARGS...>, "arguemnt error");
    }
};
}  // namespace Inner_
}  // namespace Nstd
// @@@ sample end
// @@@ sample begin 3:0

namespace Nstd {

template <typename T, size_t N>
using SafeArray
    = Nstd::SafeIndex<Nstd::Inner_::std_array_like, T, std::integral_constant<size_t, N>>;
}  // namespace Nstd
// @@@ sample end
// @@@ sample begin 4:0

namespace Nstd {

using SafeStringU16 = Nstd::SafeIndex<std::basic_string, char16_t>;
using SafeStringU32 = Nstd::SafeIndex<std::basic_string, char32_t>;
}  // namespace Nstd
// @@@ sample end
