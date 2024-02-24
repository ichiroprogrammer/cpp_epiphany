#include <cassert>
#include <cstdint>
#include <memory>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
using Yen = uint32_t;

namespace NotOCP {

// @@@ sample begin 0:0

class TransactorGoogle {
public:
    static bool Pay(Yen price) noexcept
    {
        // @@@ ignore begin
        IGNORE_UNUSED_VAR(price);
        return true;
        // @@@ ignore end
    }

    static bool Charge(Yen price) noexcept
    {
        // @@@ ignore begin
        IGNORE_UNUSED_VAR(price);
        return true;
        // @@@ ignore end
    }
};

class TransactorSuica {
    // @@@ ignore begin
public:
    static bool Pay(Yen price) noexcept
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }

    static bool Charge(Yen price) noexcept
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }
    // @@@ ignore end
};

class TransactorEdy {
public:
    // @@@ ignore begin
    static bool Pay(Yen price) noexcept
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }

    static bool Charge(Yen price) noexcept
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }
    // @@@ ignore end
};

class TransactorNotOCP {
public:
    enum class TransactionMethod { Google, Suica, Edy };

    explicit TransactorNotOCP(TransactionMethod pay_method) noexcept : pay_method_{pay_method} {}

    // @@@ ignore begin
    ~TransactorNotOCP() = default;

    // @@@ ignore end
    bool Charge(Yen price) noexcept
    {
        switch (pay_method_) {
        case TransactionMethod::Google:
            return TransactorGoogle::Charge(price);
        case TransactionMethod::Suica:
            return TransactorSuica::Charge(price);
        // @@@ ignore begin
        case TransactionMethod::Edy:
            return TransactorEdy::Charge(price);
        default:
            assert(false);
            return false;
            // @@@ ignore end
        }
    }

    bool Pay(Yen price) noexcept
    {
        switch (pay_method_) {
        case TransactionMethod::Google:
            return TransactorGoogle::Pay(price);
        case TransactionMethod::Suica:
            return TransactorSuica::Pay(price);
        // @@@ ignore begin
        case TransactionMethod::Edy:
            return TransactorEdy::Pay(price);
        default:
            assert(false);
            return false;
            // @@@ ignore end
        }
    }
    // @@@ ignore begin

private:
    TransactionMethod const pay_method_;
    // @@@ ignore end
};
// @@@ sample end
}  // namespace NotOCP

namespace OCP {
// @@@ sample begin 1:0

class TransactorIF {
public:
    // @@@ ignore begin
    TransactorIF()          = default;
    virtual ~TransactorIF() = default;

    // @@@ ignore end
    bool Charge(Yen price) noexcept { return charge(price); }
    bool Pay(Yen price) noexcept { return pay(price); }

private:
    virtual bool charge(Yen price) = 0;
    virtual bool pay(Yen price)    = 0;
};

class TransactorGoogle : public TransactorIF {
    // @@@ ignore begin
private:
    virtual bool charge(Yen price) noexcept override
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }

    virtual bool pay(Yen price) noexcept override
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }
    // @@@ ignore end
};

class TransactorSuica : public TransactorIF {
    // @@@ ignore begin
private:
    virtual bool charge(Yen price) noexcept override
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }

    virtual bool pay(Yen price) noexcept override
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }
    // @@@ ignore end
};

class TransactorEdy : public TransactorIF {
    // @@@ ignore begin
private:
    virtual bool charge(Yen price) noexcept override
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }

    virtual bool pay(Yen price) noexcept override
    {
        IGNORE_UNUSED_VAR(price);
        return true;
    }
    // @@@ ignore end
};

class TransactorOCP {
public:
    explicit TransactorOCP(std::unique_ptr<TransactorIF>&& transactor) noexcept
        : transactor_{std::move(transactor)}
    {
    }

    bool Charge(Yen price) noexcept { return transactor_->Charge(price); }

    bool Pay(Yen price) noexcept { return transactor_->Pay(price); }

private:
    std::unique_ptr<TransactorIF> transactor_;
};
// @@@ sample end

void f()
{
    TransactorOCP ocp{std::make_unique<TransactorGoogle>()};

    ocp.Charge(500);
    ocp.Pay(319);
}

TEST(OCP, do_nothing) { f(); }
}  // namespace OCP
}  // namespace
