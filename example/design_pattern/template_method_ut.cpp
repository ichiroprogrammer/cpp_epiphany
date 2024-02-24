#include "gtest_wrapper.h"

namespace ForTest {
#include "template_method.cpp"

// @@@ sample begin 0:0

TEST(TemplateMethod, xml)
{
    auto xml = XxxDataFormatterXml{};

    {
        auto const xd     = XxxData{1, 100, 10};
        auto const expect = std::string_view{
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
            "<XxxDataFormatterXml>\n"
            "<Item>\n"
            "    <XxxData a=\"1\">\n"
            "    <XxxData b=\"100\">\n"
            "    <XxxData c=\"10\">\n"
            "</Itemp>\n"
            "</XxxDataFormatterXml>\n"};
        auto const actual = xml.ToString(xd);

        ASSERT_EQ(expect, actual);
    }
    {
        auto const xds    = std::vector<XxxData>{{1, 100, 10}, {2, 200, 20}};
        auto const expect = std::string_view{
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
            "<XxxDataFormatterXml>\n"
            "<Item>\n"
            "    <XxxData a=\"1\">\n"
            "    <XxxData b=\"100\">\n"
            "    <XxxData c=\"10\">\n"
            "</Itemp>\n"
            "<Item>\n"
            "    <XxxData a=\"2\">\n"
            "    <XxxData b=\"200\">\n"
            "    <XxxData c=\"20\">\n"
            "</Itemp>\n"
            "</XxxDataFormatterXml>\n"};
        auto const actual = xml.ToString(xds);

        ASSERT_EQ(expect, actual);
    }
}

TEST(TemplateMethod, csv)
{
    auto csv = XxxDataFormatterCsv{};

    {
        auto const xd     = XxxData{1, 100, 10};
        auto const expect = std::string_view{
            "a, b, c\n"
            "1, 100, 100\n"};
        auto const actual = csv.ToString(xd);

        ASSERT_EQ(expect, actual);
    }
    {
        auto const xds    = std::vector<XxxData>{{1, 100, 10}, {2, 200, 20}};
        auto const expect = std::string_view{
            "a, b, c\n"
            "1, 100, 100\n"
            "2, 200, 200\n"};
        auto const actual = csv.ToString(xds);

        ASSERT_EQ(expect, actual);
    }
}

TEST(TemplateMethod, table)
{
    auto table = XxxDataFormatterTable{};

    // @@@ ignore begin
    {
        auto const xd     = XxxData{1, 100, 10};
        auto const expect = std::string{
            "+--------|--------|--------+\n"
            "| a      | b      | c      |\n"
            "+--------|--------|--------+\n"
            "| 1      | 100    | 10     |\n"
            "+--------|--------|--------+\n"};
        auto const actual = table.ToString(xd);

        ASSERT_EQ(expect, actual);
    }
    {
        auto const xds    = std::vector<XxxData>{{1, 100, 10}, {2, 200, 20}};
        auto const expect = std::string{
            "+--------|--------|--------+\n"
            "| a      | b      | c      |\n"
            "+--------|--------|--------+\n"
            "| 1      | 100    | 10     |\n"
            "+--------|--------|--------+\n"
            "| 2      | 200    | 20     |\n"
            "+--------|--------|--------+\n"};
        auto const actual = table.ToString(xds);

        ASSERT_EQ(expect, actual);
    }
    // @@@ ignore end
}
// @@@ sample end

namespace TemplateTemplateMethod {

// @@@ sample begin 1:0

template <typename T>  // Tは下記のXxxDataFormatterXmlのようなクラス
class XxxDataFormatter : private T {
public:
    std::string ToString(XxxData const& xxx_data) const
    {
        return T::Header() + T::Body(xxx_data) + T::Footer();
    }

    std::string ToString(std::vector<XxxData> const& xxx_datas) const
    {
        auto ret = std::string{T::Header()};

        for (auto const& xxx_data : xxx_datas) {
            ret += T::Body(xxx_data);
        }

        return ret + T::Footer();
    }
};

class XxxDataFormatterXml_Impl {
public:
    std::string const& Header() const noexcept { return header_; }
    std::string const& Footer() const noexcept { return footer_; }
    std::string        Body(XxxData const& xxx_data) const
    {
        auto content = std::string{"<Item>\n"};

        content += "    <XxxData a=\"" + std::to_string(xxx_data.a) + "\">\n";
        content += "    <XxxData b=\"" + std::to_string(xxx_data.b) + "\">\n";
        content += "    <XxxData c=\"" + std::to_string(xxx_data.c) + "\">\n";

        return content + "</Itemp>\n";
    }

private:
    inline static std::string const header_{
        "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<XxxDataFormatterXml>\n"};
    inline static std::string const footer_{"</XxxDataFormatterXml>\n"};
};

using XxxDataFormatterXml = XxxDataFormatter<XxxDataFormatterXml_Impl>;
// @@@ sample end

TEST(TemplateMethod, xml_by_template)
{
    // @@@ sample begin 1:1

    auto xml = XxxDataFormatterXml{};

    {
        auto const xd     = XxxData{1, 100, 10};
        auto const expect = std::string{
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
            "<XxxDataFormatterXml>\n"
            "<Item>\n"
            "    <XxxData a=\"1\">\n"
            "    <XxxData b=\"100\">\n"
            "    <XxxData c=\"10\">\n"
            "</Itemp>\n"
            "</XxxDataFormatterXml>\n"};
        auto const actual = xml.ToString(xd);

        ASSERT_EQ(expect, actual);
    }
    {
        auto const xds    = std::vector<XxxData>{{1, 100, 10}, {2, 200, 20}};
        auto const expect = std::string{
            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
            "<XxxDataFormatterXml>\n"
            "<Item>\n"
            "    <XxxData a=\"1\">\n"
            "    <XxxData b=\"100\">\n"
            "    <XxxData c=\"10\">\n"
            "</Itemp>\n"
            "<Item>\n"
            "    <XxxData a=\"2\">\n"
            "    <XxxData b=\"200\">\n"
            "    <XxxData c=\"20\">\n"
            "</Itemp>\n"
            "</XxxDataFormatterXml>\n"};
        auto const actual = xml.ToString(xds);

        ASSERT_EQ(expect, actual);
    }
    // @@@ sample end
}
}  // namespace TemplateTemplateMethod
}  // namespace ForTest
