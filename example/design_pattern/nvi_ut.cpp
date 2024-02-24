#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
namespace NVI_NG {
// @@@ sample begin 0:0

class Base {
public:
    virtual bool DoSomething(int something) const noexcept
    {
        // @@@ ignore begin
        IGNORE_UNUSED_VAR(something);
        return false;
        // @@@ ignore end
    }

    virtual ~Base() = default;

private:
    // @@@ ignore begin
    // @@@ ignore end
};
// @@@ sample end
// @@@ sample begin 0:1

class Derived : public Base {
public:
    virtual bool DoSomething(int something) const noexcept override
    {
        // @@@ ignore begin
        IGNORE_UNUSED_VAR(something);
        return true;
        // @@@ ignore end
    }

private:
    // @@@ ignore begin
    // @@@ ignore end
};
// @@@ sample end

TEST(NVI, base_derived_ng)
{
    Base const    b;
    Derived const d;
    Base const&   d_ref = d;

    ASSERT_FALSE(b.DoSomething(0));
    ASSERT_TRUE(d.DoSomething(0));
    ASSERT_TRUE(d_ref.DoSomething(0));
}
}  // namespace NVI_NG

namespace NVI_OK {
// @@@ sample begin 1:0

class Base {
public:
    bool DoSomething(int something) const noexcept { return do_something(something); }
    virtual ~Base() = default;

private:
    virtual bool do_something(int something) const noexcept
    {
        // @@@ ignore begin
        IGNORE_UNUSED_VAR(something);
        return false;
        // @@@ ignore end
    }

    // @@@ ignore begin
    // @@@ ignore end
};

class Derived : public Base {
private:
    virtual bool do_something(int something) const noexcept override
    {
        // @@@ ignore begin
        IGNORE_UNUSED_VAR(something);
        return true;
        // @@@ ignore end
    }

    // @@@ ignore begin
    // @@@ ignore end
};
// @@@ sample end

TEST(NVI, base_derived_ok)
{
    Base const    b;
    Derived const d;
    Base const&   d_ref = d;

    ASSERT_FALSE(b.DoSomething(0));
    ASSERT_TRUE(d.DoSomething(0));
    ASSERT_TRUE(d_ref.DoSomething(0));
}
}  // namespace NVI_OK
}  // namespace

// @@@ sample begin 2:0

class NotNviBase {
public:
    virtual std::string Name(bool mangled = false) const
    {
        return mangled ? typeid(*this).name() : "NotNviBase";
    }

    virtual ~NotNviBase() = default;
};

class NotNviDerived : public NotNviBase {
public:
    virtual std::string Name(bool mangled = true) const override  // NG デフォルト値が違う
    {
        return mangled ? typeid(*this).name() : "NotNviDerived";
    }
};
// @@@ sample end

namespace {
TEST(NVI, base_derived_ng2)
{
    // @@@ sample begin 2:1

    NotNviDerived const d;
    NotNviBase const&   d_ref = d;

    ASSERT_EQ("NotNviDerived", d.Name(false));   // OK
    ASSERT_EQ("13NotNviDerived", d.Name(true));  // OK

    ASSERT_EQ("NotNviDerived", d_ref.Name(false));   // OK
    ASSERT_EQ("13NotNviDerived", d_ref.Name(true));  // OK

    ASSERT_EQ("13NotNviDerived", d.Name());    // mangled == false
    ASSERT_EQ("NotNviDerived", d_ref.Name());  // mangled == true

    ASSERT_NE(d.Name(), d_ref.Name());  // NG d_refの実態はdであるが、d.Name()と動きが違う
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 3:0
class NviBase {
public:
    std::string Name(bool mangled = false) const { return name(mangled); }
    virtual ~NviBase() = default;

private:
    virtual std::string name(bool mangled) const
    {
        return mangled ? typeid(*this).name() : "NviBase";
    }
};

class NviDerived : public NviBase {
private:
    virtual std::string name(bool mangled) const override  // OK デフォルト値を持たない
    {
        return mangled ? typeid(*this).name() : "NviDerived";
    }
};
// @@@ sample end

namespace {
TEST(NVI, base_derived_ok2)
{
    // @@@ sample begin 3:1

    NviBase const    b;
    NviDerived const d;
    NviBase const&   d_ref = d;

    ASSERT_EQ("NviDerived", d.Name(false));   // OK
    ASSERT_EQ("10NviDerived", d.Name(true));  // OK

    ASSERT_EQ("NviDerived", d_ref.Name(false));   // OK
    ASSERT_EQ("10NviDerived", d_ref.Name(true));  // OK

    ASSERT_EQ("NviDerived", d.Name());      // mangled == false
    ASSERT_EQ("NviDerived", d_ref.Name());  // mangled == false

    ASSERT_EQ(d.Name(), d_ref.Name());  // OK
    // @@@ sample end
}
}  // namespace
