#pragma once
#include <vector>

// @@@ sample begin 0:0

namespace Nstd {

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& vec)
{
    auto first = true;

    for (auto const& i : vec) {
        if (!std::exchange(first, false)) {
            os << ", ";
        }
        os << i;
    }

    return os;
}

template <typename T>
std::string ToString(std::vector<T> const& vec)
{
    auto oss = std::ostringstream{};

    oss << vec;

    return oss.str();
}
}  // namespace Nstd
// @@@ sample end
