#pragma once

#include <string>
#include <vector>

// @@@ sample begin 0:0

/// @class XxxData
/// @brief 何かのデータを入れる箱
struct XxxData {
    int a;
    int b;
    int c;
};
// @@@ sample end
// @@@ sample begin 0:1

/// @class XxxDataFormatterIF
/// @brief data_storer_if.cppに定義すべきだが、サンプルであるため便宜上同じファイルで定義する
///        データフォーマットを行うクラスのインターフェースクラス
class XxxDataFormatterIF {
public:
    explicit XxxDataFormatterIF(std::string_view formatter_name) noexcept
        : formatter_name_{formatter_name}
    {
    }
    virtual ~XxxDataFormatterIF() = default;
    // @@@ sample end
    // @@@ sample begin 0:2

    static XxxDataFormatterIF const& Xml() noexcept;
    static XxxDataFormatterIF const& Csv() noexcept;
    static XxxDataFormatterIF const& Table() noexcept;
    // @@@ sample end
    // @@@ sample begin 0:3

    std::string ToString(XxxData const& xxx_data) const
    {
        return header() + body(xxx_data) + footer();
    }

    std::string ToString(std::vector<XxxData> const& xxx_datas) const
    {
        std::string ret{header()};

        for (auto const& xxx_data : xxx_datas) {
            ret += body(xxx_data);
        }

        return ret + footer();
    }
    // @@@ ignore begin

    std::string_view FormatterName() const noexcept { return formatter_name_; }
    // @@@ ignore end
private:
    virtual std::string const& header() const                      = 0;
    virtual std::string const& footer() const                      = 0;
    virtual std::string        body(XxxData const& xxx_data) const = 0;
    // @@@ sample end
    // @@@ sample begin 0:4

    // @@@ ignore begin
    std::string_view const formatter_name_;

    XxxDataFormatterIF(XxxDataFormatterIF const&)            = delete;
    XxxDataFormatterIF& operator=(XxxDataFormatterIF const&) = delete;
    XxxDataFormatterIF(XxxDataFormatterIF&&)                 = delete;
    XxxDataFormatterIF& operator=(XxxDataFormatterIF&&)      = delete;
    // @@@ ignore end
};
// @@@ sample end
// @@@ sample begin 1:0

enum class XxxDataFormatterMethod {
    Xml,
    Csv,
    Table,
};

/// @fn XxxDataFormatterFactory
/// @brief std::unique_ptrで保持されたXxxDataFormatterIFオブジェクトを生成するFactory関数
/// @param method XxxDataFormatterMethodのいずれか
/// @return std::unique_ptr<const XxxDataFormatterIF>
///         XxxDataFormatterIFはconstメンバ関数のみを持つため、戻り値もconstオブジェクト
std::unique_ptr<XxxDataFormatterIF const> XxxDataFormatterFactory(XxxDataFormatterMethod method);
// @@@ sample end

/// @fn XxxDataFormatterFactory2
/// @brief const XxxDataFormatterIFオブジェクトのリファレンスを返すFactory関数
/// @param method XxxDataFormatterMethodのいずれか
/// @return const XxxDataFormatterIF
///         XxxDataFormatterIFはconstメンバ関数のみを持つため、戻り値もconstオブジェクト
XxxDataFormatterIF const& XxxDataFormatterFactory2(XxxDataFormatterMethod method) noexcept;
