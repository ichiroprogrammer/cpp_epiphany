#pragma once

// @@@ sample begin 0:0

class test_class_exits_put_to {
public:
    test_class_exits_put_to(int i = 0) noexcept : i_{i} {}
    int get() const noexcept { return i_; }

private:
    int i_;
};

inline std::ostream& operator<<(std::ostream& os, test_class_exits_put_to const& p)
{
    return os << p.get();
}

class test_class_not_exits_put_to {};
// @@@ sample end
