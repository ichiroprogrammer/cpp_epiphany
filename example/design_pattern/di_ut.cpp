#include <memory>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

/// @class Depended
/// @brief NotDIや、DIから依存されるクラス
class Depended {
    // @@@ ignore begin
public:
    Depended()          = default;
    virtual ~Depended() = default;
    void DoSomething() { do_something(); }

private:
    virtual void do_something()  // 何かやる
    {
    }
    // @@@ ignore end
};

/// @class NotDI
/// @brief NotDIを使わない例。そのため、NotDIは、Dependedのインスタンスに依存している。
class NotDI {
public:
    NotDI() : not_di_depended_{std::make_unique<Depended>()} {}

    void DoSomething() { not_di_depended_->DoSomething(); }

private:
    std::unique_ptr<Depended> not_di_depended_;
};
// @@@ sample end

// @@@ sample begin 0:1

/// @class DI
/// @brief DIを使う例。そのため、DIは、Dependedの型に依存している。
class DI {
public:
    explicit DI(std::unique_ptr<Depended>&& di_depended) noexcept
        : di_depended_{std::move(di_depended)}
    {
    }

    void DoSomething() { di_depended_->DoSomething(); }

private:
    std::unique_ptr<Depended> di_depended_;
};
// @@@ sample end

TEST(DI, just_do_it)
{
    auto not_di = NotDI{};

    not_di.DoSomething();

    auto di = DI{std::make_unique<Depended>()};

    di.DoSomething();
}
}  // namespace

namespace NoUseDI {
// @@@ sample begin 1:0

class DeviceIO {
public:
    uint8_t read()
    {
        uint8_t ret{0};  // @@@ delete
        // ハードウェアに依存した何らかの処理
        return ret;  // @@@ delete
    }

    void write(uint8_t a)
    {
        // ハードウェアに依存した何らかの処理
        IGNORE_UNUSED_VAR(a);  // @@@ delete
    }

private:
    // 何らかの宣言
};

#ifdef UNIT_TEST       // 単体テストビルドでは定義されるマクロ
class DeviceIO_Mock {  // 単体テスト用のモック
public:
    uint8_t read()
    {
        uint8_t ret{0};  // @@@ delete
        // ハードウェアに依存しない何らかの処理
        return ret;  // @@@ delete
    }

    void write(uint8_t a)
    {
        // ハードウェアに依存しない何らかの処理
        IGNORE_UNUSED_VAR(a);  // @@@ delete
    }

private:
    // 何らかの宣言
};
#endif
// @@@ sample end
// @@@ sample begin 1:1

class Widget {
public:
    void DoSomething()
    {
        // io_を使った何らかの処理
    }

    uint8_t GetResp()
    {
        // io_を使った何らかの処理
        return io_.read();  // @@@ delete
    }

private:
#ifdef UNIT_TEST
    DeviceIO_Mock io_;
#else
    DeviceIO io_;
#endif
};
// @@@ sample end

TEST(DI, ifdef_unit_test)
{
    // @@@ sample begin 1:2

    // UNIT_TESTマクロが定義されたWidgetの単体テスト
    Widget w;

    w.DoSomething();
    ASSERT_EQ(0, w.GetResp());
    // @@@ sample end
}
}  // namespace NoUseDI
namespace UseDI_0 {
// @@@ sample begin 2:0

class DeviceIO {
public:
    virtual uint8_t read()  // モックでオーバーライドするためvirtual
    {
        uint8_t ret{0};  // @@@ delete
        // ハードウェアに依存した何らかの処理
        return ret;  // @@@ delete
    }

    virtual void write(uint8_t a)  // モックでオーバーライドするためvirtual
    {
        // ハードウェアに依存した何らかの処理
        IGNORE_UNUSED_VAR(a);  // @@@ delete
    }
    virtual ~DeviceIO() = default;

private:
    // 何らかの宣言
};
// @@@ sample end
// @@@ sample begin 2:1

class Widget {
public:
    Widget(std::unique_ptr<DeviceIO> io = std::make_unique<DeviceIO>()) : io_{std::move(io)} {}

    void DoSomething()
    {
        // io_を使った何らかの処理
    }

    uint8_t GetResp()
    {
        // io_を使った何らかの処理
        return io_->read();  // @@@ delete
    }

private:
    std::unique_ptr<DeviceIO> io_;
};
// @@@ sample end
// @@@ sample begin 2:2

class DeviceIO_Mock : public DeviceIO {  // 単体テスト用のモック
public:
    uint8_t read() override
    {
        uint8_t ret{1};  // @@@ delete
        // ハードウェアに依存しない何らかの処理
        return ret;  // @@@ delete
    }

    void write(uint8_t a) override
    {
        // ハードウェアに依存しない何らかの処理
        IGNORE_UNUSED_VAR(a);  // @@@ delete
    }

private:
    // 何らかの宣言
};
// @@@ sample end

TEST(DI, use_di_0)
{
    // @@@ sample begin 2:3

    Widget w{std::unique_ptr<DeviceIO>(new DeviceIO_Mock)};  // モックのインジェクション

    // Widgetの単体テスト
    w.DoSomething();
    ASSERT_EQ(1, w.GetResp());
    // @@@ sample end
}
}  // namespace UseDI_0
namespace UseDI_1 {
// @@@ sample begin 3:0

class DeviceIO {
public:
    uint8_t read()  // Widgetがテンプレートであるため非virtualで良い
    {
        uint8_t ret{0};  // @@@ delete
        // ハードウェアに依存した何らかの処理
        return ret;  // @@@ delete
    }

    void write(uint8_t a)  // Widgetがテンプレートであるため非virtualで良い
    {
        // ハードウェアに依存した何らかの処理
        IGNORE_UNUSED_VAR(a);  // @@@ delete
    }
    virtual ~DeviceIO() = default;

private:
    // 何らかの宣言
};
// @@@ sample end
// @@@ sample begin 3:1

template <class T = DeviceIO>
class Widget {
public:
    void DoSomething()
    {
        // io_を使った何らかの処理
    }

    uint8_t GetResp()
    {
        // io_を使った何らかの処理
        return io_.read();  // @@@ delete
    }

private:
    T io_;
};
// @@@ sample end
// @@@ sample begin 3:2

class DeviceIO_Mock {  // 単体テスト用のモック
public:
    uint8_t read()  // Widgetがテンプレートであるため非virtualで良い
    {
        uint8_t ret{2};  // @@@ delete
        // ハードウェアに依存しない何らかの処理
        return ret;  // @@@ delete
    }

    void write(uint8_t a)  // Widgetがテンプレートであるため非virtualで良い
    {
        // ハードウェアに依存しない何らかの処理
        IGNORE_UNUSED_VAR(a);  // @@@ delete
    }

private:
    // 何らかの宣言
};
// @@@ sample end

TEST(DI, use_di_1)
{
    // @@@ sample begin 3:3

    Widget<DeviceIO_Mock> w;

    // Widget<>の単体テスト
    w.DoSomething();
    ASSERT_EQ(2, w.GetResp());
    // @@@ sample end
}
}  // namespace UseDI_1
