#include <algorithm>
#include <iostream>

#include "find_files_strategy.h"

// @@@ sample begin 0:0

std::vector<std::string> find_files_recursively(std::string const& path, find_condition condition)
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
