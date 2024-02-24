#include <fstream>

#include "gtest_wrapper.h"

#include "find_files_old_style.h"
#include "find_files_strategy.h"

namespace {

std::string const test_dir{"find_files_ut_dir/"};

void assure_test_files_exist()
{
    // test用のファイルがあることの確認
    ASSERT_TRUE(std::ofstream{test_dir + "dir0/file2"}.is_open());
    ASSERT_TRUE(std::ofstream{test_dir + "dir0/gile3"}.is_open());
    ASSERT_TRUE(std::ofstream{test_dir + "dir1/dir2/file4"}.is_open());
    ASSERT_TRUE(std::ofstream{test_dir + "file0"}.is_open());
    ASSERT_TRUE(std::ofstream{test_dir + "gile1"}.is_open());
}

template <typename T>
std::vector<T> sort(std::vector<T>&& v)
{
    std::sort(v.begin(), v.end());
    return v;
}

// @@@ sample begin 0:0

TEST(Strategy, old_style)
{
    assure_test_files_exist();  // test用のファイルがあることの確認

    auto const files_actual = find_files_recursively(test_dir, FindCondition::File);
    auto const files_expect = sort(std::vector{
        // clang-format off
        test_dir + "dir0/file2",
        test_dir + "dir0/gile3",
        test_dir + "dir1/dir2/file4",
        test_dir + "file0",
        test_dir + "gile1"
        // clang-format on
    });
    ASSERT_EQ(files_expect, files_actual);

    auto const dirs_actual = find_files_recursively(test_dir, FindCondition::Dir);
    auto const dirs_expect = sort(std::vector{
        // clang-format off
        test_dir + "dir0",
        test_dir + "dir1",
        test_dir + "dir1/dir2"
        // clang-format on
    });
    ASSERT_EQ(dirs_expect, dirs_actual);

    auto const f_actual = find_files_recursively(test_dir, FindCondition::FileNameHeadIs_f);
    auto const f_expect = sort(std::vector{
        // clang-format off
        test_dir + "dir0/file2",
        test_dir + "dir1/dir2/file4",
        test_dir + "file0"
        // clang-format on
    });
    ASSERT_EQ(f_expect, f_actual);
}
// @@@ sample end

// @@@ sample begin 0:1

TEST(Strategy, strategy_lamda)
{
    namespace fs = std::filesystem;

    assure_test_files_exist();  // test用のファイルがあることの確認

    // ラムダ式で実装
    auto const files_actual = find_files_recursively(
        test_dir, [](fs::path const& p) noexcept { return fs::is_regular_file(p); });

    auto const files_expect = sort(std::vector{
        // clang-format off
        test_dir + "dir0/file2",
        test_dir + "dir0/gile3",
        test_dir + "dir1/dir2/file4",
        test_dir + "file0",
        test_dir + "gile1"
        // clang-format on
    });
    ASSERT_EQ(files_expect, files_actual);

    auto const dirs_actual = find_files_recursively(
        test_dir, [](fs::path const& p) noexcept { return fs::is_directory(p); });
    auto const dirs_expect = sort(std::vector{
        // clang-format off
        test_dir + "dir0",
        test_dir + "dir1",
        test_dir + "dir1/dir2"
        // clang-format on
    });
    ASSERT_EQ(dirs_expect, dirs_actual);

    auto const f_actual = find_files_recursively(test_dir, [](fs::path const& p) noexcept {
        return p.filename().generic_string()[0] == 'f';
    });

    auto const f_expect = sort(std::vector{
        // clang-format off
        test_dir + "dir0/file2",
        test_dir + "dir1/dir2/file4",
        test_dir + "file0"
        // clang-format on
    });
    ASSERT_EQ(f_expect, f_actual);
}

/// @fn bool condition_func(std::filesystem::path const& path)
/// @brief find_files_recursivelyの第2仮引数に渡すためのファイル属性を決める関数
bool condition_func(std::filesystem::path const& path)
{
    return path.filename().generic_string().at(0) == 'f';
}

TEST(Strategy, strategy_func_pointer)
{
    assure_test_files_exist();  // test用のファイルがあることの確認

    // FindCondition::FileNameHeadIs_fで行ったことを関数ポインタで実装。
    auto const f_actual = find_files_recursively(test_dir, condition_func);
    auto const f_expect = sort(std::vector{
        // clang-format off
        test_dir + "dir0/file2",
        test_dir + "dir1/dir2/file4",
        test_dir + "file0"
        // clang-format on
    });
    ASSERT_EQ(f_expect, f_actual);
}

/// @class ConditionFunctor
/// @brief
///  find_files_recursivelyの第2仮引数に渡すためのファイル属性を決める関数オブジェクトクラス。
///  検索条件に状態が必要な場合、関数オブジェクトを使うとよい。
class ConditionFunctor {
public:
    ConditionFunctor()  = default;
    ~ConditionFunctor() = default;

    /// @fn bool operator()(std::filesystem::path const& path)
    /// @brief 先頭が'f'のファイルを最大2つまで探す
    bool operator()(std::filesystem::path const& path)
    {
        if (path.filename().generic_string().at(0) != 'f') {
            return false;
        }

        return ++count_ < 3;
    }

private:
    int32_t count_{0};
};

TEST(Strategy, strategy_func_obj)
{
    // 条件に状態が必要な場合(この例では最大2つまでを判断するのに状態が必要)、
    // 関数ポインタより、ファンクタの方が便利。
    auto const f_actual = find_files_recursively(test_dir, ConditionFunctor{});
    auto const f_expect = sort(std::vector{
        test_dir + "dir0/file2",
        test_dir + "dir1/dir2/file4",
    });
    ASSERT_EQ(f_expect, f_actual);
}
// @@@ sample end

TEST(Strategy, strategy_func_obj2)
{
    // 条件に状態が必要な場合(この例では最大2つまでを判断するのに状態が必要)、
    // 関数ポインタより、ファンクタの方が便利。
    auto const f_actual = find_files_recursively2(test_dir, ConditionFunctor{});
    auto const f_expect = sort(std::vector{
        test_dir + "dir0/file2",
        test_dir + "dir1/dir2/file4",
    });
    ASSERT_EQ(f_expect, f_actual);
}
}  // namespace
