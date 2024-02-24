#include <algorithm>
#include <cassert>
#include <filesystem>

#include "find_files_old_style.h"

namespace fs = std::filesystem;

// @@@ sample begin 0:0

/// @fn std::vector<std::string> find_files_recursively(std::string const& path,
///                                                     FindCondition condition)
/// @brief 条件にマッチしたファイルをリカーシブに探して返す
/// @param path      リカーシブにディレクトリをたどるための起点となるパス
/// @param condition どのようなファイルかを指定する
/// @return 条件にマッチしたファイルをstd::vector<std::string>で返す
std::vector<std::string> find_files_recursively(std::string const& path, FindCondition condition)
{
    auto files = std::vector<fs::path>{};

    // recursive_directory_iteratorはファイルシステム依存するため、その依存を排除する他の処理
    std::copy(fs::recursive_directory_iterator{path}, fs::recursive_directory_iterator{},
              std::back_inserter(files));

    std::sort(files.begin(), files.end());

    auto ret = std::vector<std::string>{};

    std::for_each(files.begin(), files.end(), [&](fs::path const& p) noexcept {
        auto is_match = false;

        switch (condition) {
        case FindCondition::File:
            if (fs::is_regular_file(p)) {
                is_match = true;
            }
            break;
        case FindCondition::Dir:
            if (fs::is_directory(p)) {
                is_match = true;
            }
            break;
        // @@@ ignore begin
        case FindCondition::FileNameHeadIs_f:
            if (p.filename().generic_string().at(0) == 'f') {
                is_match = true;
            }
            break;
        default:
            assert(false);
            // @@@ ignore end
        }

        if (is_match) {
            ret.emplace_back(p.generic_string());
        }
    });

    return ret;
}
// @@@ sample end
