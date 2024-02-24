#include <string>
#include <vector>

// @@@ sample begin 0:0

/// @enum FindCondition
/// find_files_recursivelyの条件
enum class FindCondition {
    File,              ///< pathがファイル
    Dir,               ///< pathがディレクトリ
    FileNameHeadIs_f,  ///< pathがファイル且つ、そのファイル名の先頭が"f"
};
// @@@ sample end

extern std::vector<std::string> find_files_recursively(std::string const& path,
                                                       FindCondition      condition);
