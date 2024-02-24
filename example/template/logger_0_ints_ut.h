// 本来ならこんなファイルは作るべきではないが、編集都合上やむを得ない。
// INTS_UT_NAMEを#defineして使う。
namespace {
TEST(Template, INTS_UT_NAME)
{
    Logging::Logger::Inst().Clear();

    // @@@ sample begin 0:0

    auto ints = App::Ints_t{1, 2, 3};

    LOGGER("Ints", ints);
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":Ints:1, 2, 3\n")};
    ASSERT_EQ(exp, s);
    // @@@ sample end
}
}  // namespace
