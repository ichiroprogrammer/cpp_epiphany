#pragma once

#include <ostream>
#include <vector>

// @@@ sample begin 0:0

namespace App {
using Ints_t = std::vector<int>;
}
// @@@ sample end
// @@@ sample begin 1:0

namespace App {

class X {
public:
    X(std::string str, int num) : str_{std::move(str)}, num_{num} {}
    std::string ToString() const { return str_ + "/" + std::to_string(num_); }
    // @@@ ignore begin
private:
    std::string const str_;
    int const         num_;
    // @@@ ignore end
};
}  // namespace App
// @@@ sample end
// @@@ sample begin 1:1

namespace App {

inline std::ostream& operator<<(std::ostream& os, X const& x) { return os << x.ToString(); }
}  // namespace App
// @@@ sample end
