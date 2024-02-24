#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace ImplicitConversion {
// @@@ sample begin 0:0

class Person {
public:
    Person(char const* name, uint32_t age = 0) : name_{name}, age_{age} {}
    Person(Person const&)            = default;
    Person& operator=(Person const&) = default;

    std::string const& GetName() const noexcept { return name_; }
    uint32_t           GetAge() const noexcept { return age_; }

private:
    std::string name_;  // コピーをするため非const
    uint32_t    age_;
};

bool operator==(Person const& lhs, Person const& rhs) noexcept
{
    return (lhs.GetName() == rhs.GetName()) && (lhs.GetAge() == rhs.GetAge());
}
// @@@ sample end
// @@@ sample begin 0:1

void f(Person const& person) noexcept
{
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(person);
    // @@@ ignore end
}

void using_implicit_coversion()
{
    f("Ohtani");  // "Ohtani"はPerson型ではないが、コンパイル可能
}
// @@@ sample end
// @@@ sample begin 0:2

void not_using_implicit_coversion()
{
    f(Person{"Ohtani"});  // 本来は、fの引数はPerson型
}
// @@@ sample end

namespace {
TEST(ImplicitConversion, implicit_conversion)
{
    {
        // @@@ sample begin 0:3

        auto otani = std::string{"Ohtani"};

        // @@@ ignore begin
        // @@@ ignore end

        if (otani == "Ohtani") {  // 暗黙の型変換によりコンパイルできる
            // @@@ ignore begin
            // @@@ ignore end
        }
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:4

        auto otani = Person{"Ohtani", 26};

        // @@@ ignore begin
        // @@@ ignore end

        if (otani == "Otani") {  // このコードがコンパイルされる。
            // @@@ ignore begin
            // @@@ ignore end
        }
        // @@@ sample end
    }
}
}  // namespace
}  // namespace ImplicitConversion

namespace ExplicitConstructor {

class Person;
void f(Person const& person) noexcept
{
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(person);
    // @@@ ignore end
}

// @@@ sample begin 0:5

class Person {
public:
    explicit Person(char const* name, uint32_t age = 0) : name_{name}, age_{age} {}
    Person(Person const&)            = default;
    Person& operator=(Person const&) = default;

    // @@@ ignore begin
    std::string const& GetName() const noexcept { return name_; }
    uint32_t           GetAge() const noexcept { return age_; }

private:
    std::string name_;
    uint32_t    age_;

    friend bool operator==(Person const& lhs, Person const& rhs) noexcept
    {
        return (lhs.GetName() == rhs.GetName()) && (lhs.GetAge() == rhs.GetAge());
    }
    // @@@ ignore end
};

void prohibit_implicit_coversion()
{
#if 0  // explicit付きのコンストラクタを持つPersonと違い、コンパイルできない。
    f("Ohtani");
#else
    f(Person{"Ohtani"});
#endif

    auto otani = Person{"Ohtani", 26};

    // @@@ ignore begin
    // @@@ ignore end

#if 0
    if (otani == "Otani") {  // このコードもコンパイルできない。
        // @@@ ignore begin
        // @@@ ignore end
    }
#else
    if (otani == Person{"Otani", 26}) {  // この記述を強制できる。
        // @@@ ignore begin
        // @@@ ignore end
    }
#endif
}
// @@@ sample end
}  // namespace ExplicitConstructor
