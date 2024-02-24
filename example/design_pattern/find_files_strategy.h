#include <algorithm>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

// @@@ sample begin 0:0

/// @typedef find_condition
/// @brief find_files_recursively仮引数conditionの型(関数オブジェクトの型)
using find_condition = std::function<bool(std::filesystem::path const&)>;

// Strategyパターン
/// @fn std::vector<std::string> find_files_recursively(std::string const& path,
///                                                     find_condition     condition);
/// @brief 条件にマッチしたファイルをリカーシブに探索して返す
/// @param path      リカーシブにディレクトリを辿るための起点となるパス
/// @param condition 探索するファイルの条件
/// @return 条件にマッチしたファイルをstd::vector<std::string>で返す
extern std::vector<std::string> find_files_recursively(std::string const& path,
                                                       find_condition     condition);
// @@@ sample end
// @@@ sample begin 1:0

template <typename F>  // Fはファンクタ
auto find_files_recursively2(std::string const& path, F condition)
    -> std::enable_if_t<std::is_invocable_r_v<bool, F, std::filesystem::path const&>,
                        std::vector<std::string>>
{
    namespace fs = std::filesystem;

    auto files = std::vector<fs::path>{};

    // recursive_directory_iteratorはファイルシステム依存するため、その依存を排除する他の処理
    std::copy(fs::recursive_directory_iterator{path}, fs::recursive_directory_iterator{},
              std::back_inserter(files));

    std::sort(files.begin(), files.end());

    auto ret = std::vector<std::string>{};

    std::for_each(files.cbegin(), files.cend(), [&](fs::path const& p) {
        if (condition(p)) {
            ret.emplace_back(p.generic_string());
        }
    });

    return ret;
}
// @@@ sample end
