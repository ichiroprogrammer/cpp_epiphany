#include "gtest_wrapper.h"

#include "suppress_warning.h"
#include "template_method.h"

namespace {
// @@@ sample begin 0:0

TEST(Factory, xml)
{
    auto xml = XxxDataFormatterFactory(XxxDataFormatterMethod::Xml);

    // @@@ ignore begin
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
        auto const actual = xml->ToString(xd);

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
        auto const actual = xml->ToString(xds);

        ASSERT_EQ(expect, actual);
    }
    // @@@ ignore end
}

TEST(Factory, csv)
{
    auto csv = XxxDataFormatterFactory(XxxDataFormatterMethod::Csv);

    // @@@ ignore begin
    {
        auto const xd     = XxxData{1, 100, 10};
        auto const expect = std::string{
            "a, b, c\n"
            "1, 100, 100\n"};
        auto const actual = csv->ToString(xd);

        ASSERT_EQ(expect, actual);
    }
    {
        auto const xds    = std::vector<XxxData>{{1, 100, 10}, {2, 200, 20}};
        auto const expect = std::string_view{
            "a, b, c\n"
            "1, 100, 100\n"
            "2, 200, 200\n"};
        auto const actual = csv->ToString(xds);

        ASSERT_EQ(expect, actual);
    }
    // @@@ ignore end
}

TEST(Factory, table)
{
    auto table = XxxDataFormatterFactory(XxxDataFormatterMethod::Table);

    {
        auto const xd     = XxxData{1, 100, 10};
        auto const expect = std::string_view{
            "+--------|--------|--------+\n"
            "| a      | b      | c      |\n"
            "+--------|--------|--------+\n"
            "| 1      | 100    | 10     |\n"
            "+--------|--------|--------+\n"};
        auto const actual = table->ToString(xd);

        ASSERT_EQ(expect, actual);
    }
    {
        auto const xds    = std::vector<XxxData>{{1, 100, 10}, {2, 200, 20}};
        auto const expect = std::string_view{
            "+--------|--------|--------+\n"
            "| a      | b      | c      |\n"
            "+--------|--------|--------+\n"
            "| 1      | 100    | 10     |\n"
            "+--------|--------|--------+\n"
            "| 2      | 200    | 20     |\n"
            "+--------|--------|--------+\n"};
        auto const actual = table->ToString(xds);

        ASSERT_EQ(expect, actual);
    }
}
// @@@ sample end

TEST(Factory, csv2)
{
    auto const& csv = XxxDataFormatterFactory2(XxxDataFormatterMethod::Csv);

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

TEST(Factory, table2)
{
    auto const& table = XxxDataFormatterIF::Table();

    {
        auto const xd     = XxxData{1, 100, 10};
        auto const expect = std::string_view{
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
        auto const expect = std::string_view{
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
}
}  // namespace
