#include <cassert>
#include <iostream>
#include <memory>

#include "template_method.h"

namespace {
// @@@ sample begin 0:0

/// @class XxxDataFormatterXml
/// @brief XxxDataをXmlに変換
class XxxDataFormatterXml final : public XxxDataFormatterIF {
    // @@@ ignore begin
public:
    XxxDataFormatterXml() noexcept : XxxDataFormatterIF{"Xml"} {}
    virtual ~XxxDataFormatterXml() override = default;

    // @@@ ignore end
private:
    virtual std::string const& header() const noexcept final { return header_; }
    virtual std::string const& footer() const noexcept final { return footer_; }
    virtual std::string        body(XxxData const& xxx_data) const override
    {
        auto content = std::string{"<Item>\n"};

        content += "    <XxxData a=\"" + std::to_string(xxx_data.a) + "\">\n";
        content += "    <XxxData b=\"" + std::to_string(xxx_data.b) + "\">\n";
        content += "    <XxxData c=\"" + std::to_string(xxx_data.c) + "\">\n";

        return content + "</Itemp>\n";
    }

    static inline std::string const header_{
        "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<XxxDataFormatterXml>\n"};
    static inline std::string const footer_{"</XxxDataFormatterXml>\n"};
};

/// @class XxxDataFormatterCsv
/// @brief XxxDataをCsvに変換
class XxxDataFormatterCsv final : public XxxDataFormatterIF {
    // @@@ ignore begin
public:
    XxxDataFormatterCsv() noexcept : XxxDataFormatterIF{"Csv"} {}

    virtual ~XxxDataFormatterCsv() override = default;

    // @@@ ignore end
private:
    virtual std::string const& header() const noexcept final { return header_; }
    virtual std::string const& footer() const noexcept final { return footer_; }
    virtual std::string        body(XxxData const& xxx_data) const override
    {
        return std::string{std::to_string(xxx_data.a) + ", " + std::to_string(xxx_data.b) + ", "
                           + std::to_string(xxx_data.b) + "\n"};
    }

    static inline std::string const header_{"a, b, c\n"};
    static inline std::string const footer_{};
};

/// @class XxxDataFormatterTable
/// @brief XxxDataをTableに変換
class XxxDataFormatterTable final : public XxxDataFormatterIF {
    // @@@ ignore begin
public:
    XxxDataFormatterTable() noexcept : XxxDataFormatterIF{"Table"} {}

    virtual ~XxxDataFormatterTable() override = default;

    // @@@ ignore end
private:
    virtual std::string const& header() const noexcept final { return header_; }
    virtual std::string const& footer() const noexcept final { return footer_; }
    virtual std::string        body(XxxData const& xxx_data) const override
    {
        auto a = std::string{std::string{"| "} + std::to_string(xxx_data.a)};
        auto b = std::string{std::string{"| "} + std::to_string(xxx_data.b)};
        auto c = std::string{std::string{"| "} + std::to_string(xxx_data.c)};

        a += std::string(colomun_ - a.size() + 1, ' ');
        b += std::string(colomun_ - b.size() + 1, ' ');
        c += std::string(colomun_ - c.size() + 1, ' ');

        return a + b + c + "|\n" + border_;
    }
    // @@@ ignore begin

    static std::string border()
    {
        auto border = std::string{"+"};

        border += std::string(colomun_, '-') + "|";
        border += std::string(colomun_, '-') + "|";
        border += std::string(colomun_, '-') + "+\n";

        return border;
    }

    static constexpr size_t         colomun_{8};
    static inline std::string const border_{XxxDataFormatterTable::border()};
    static inline std::string const header_{border_ + "| a      | b      | c      |\n"
                                            + XxxDataFormatterTable::border_};
    static inline std::string const footer_{};
    // @@@ ignore end
};
// @@@ sample end
}  // namespace

// @@@ sample begin 1:0

std::unique_ptr<XxxDataFormatterIF const> XxxDataFormatterFactory(XxxDataFormatterMethod method)
{
    switch (method) {
    case XxxDataFormatterMethod::Xml:
        return std::unique_ptr<XxxDataFormatterIF const>{new XxxDataFormatterXml};  // C++11
    case XxxDataFormatterMethod::Csv:
        return std::make_unique<XxxDataFormatterCsv const>();  // C++14 make_uniqueもFactory
    case XxxDataFormatterMethod::Table:
        return std::make_unique<XxxDataFormatterTable const>();
    default:
        assert(false);
        return {};
    }
}
// @@@ sample end
// @@@ sample begin 1:1

XxxDataFormatterIF const& XxxDataFormatterFactory2(XxxDataFormatterMethod method) noexcept
{
    static auto xml   = XxxDataFormatterXml{};
    static auto csv   = XxxDataFormatterCsv{};
    static auto table = XxxDataFormatterTable{};

    switch (method) {
    case XxxDataFormatterMethod::Xml:
        return xml;
    case XxxDataFormatterMethod::Csv:
        return csv;
    case XxxDataFormatterMethod::Table:
        return table;
    default:
        assert(false);
        return xml;
    }
}
// @@@ sample end
// @@@ sample begin 2:0

XxxDataFormatterIF const& XxxDataFormatterIF::Xml() noexcept
{
    static auto xml = XxxDataFormatterXml{};

    return xml;
}

XxxDataFormatterIF const& XxxDataFormatterIF::Csv() noexcept
{
    static auto csv = XxxDataFormatterCsv{};

    return csv;
}

XxxDataFormatterIF const& XxxDataFormatterIF::Table() noexcept
{
    static auto table = XxxDataFormatterTable{};

    return table;
}
// @@@ sample end
