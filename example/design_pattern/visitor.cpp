#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <typeinfo>

#include "gtest_wrapper.h"

#include "visitor.h"

namespace {
std::string find_files(std::string const& dir)
{
    namespace fs = std::filesystem;  // OK 長い名前を短く

    auto files = std::vector<std::string>{};
    auto top   = fs::path{dir};

    std::for_each(fs::recursive_directory_iterator{top}, fs::recursive_directory_iterator{},
                  [&files](fs::path const& p) { files.emplace_back(p.generic_string()); });

    std::sort(files.begin(), files.end());

    auto ret = std::string{};

    for (auto f : files) {
        if (ret.size() != 0) {
            ret += "," + f;
        }
        else {
            ret = f;
        }
    }

    return ret;
}
}  // namespace

namespace WorstPattern {

// @@@ sample begin 0:0

/// @class FileEntity
/// @brief
///  ファイルシステムの構成物(ファイル、ディレクトリ等)を表すクラスの基底クラス
class FileEntity {
public:
    explicit FileEntity(std::string pathname) : pathname_{std::move(pathname)} {}
    virtual ~FileEntity() {}
    std::string const& Pathname() const { return pathname_; }

    // @@@ ignore begin
    // @@@ ignore end

private:
    std::string const pathname_;
};

class File final : public FileEntity {
    // @@@ ignore begin
public:
    using FileEntity::FileEntity;
    // @@@ ignore end
};

class Dir final : public FileEntity {
    // @@@ ignore begin
public:
    using FileEntity::FileEntity;
    // @@@ ignore end
};

class OtherEntity final : public FileEntity {
    // @@@ ignore begin
public:
    using FileEntity::FileEntity;
    // @@@ ignore end
};

class Printer {
public:
    static void PrintPathname1(FileEntity const& file_entity)
    {
        if (typeid(File) == typeid(file_entity)) {
            std::cout << file_entity.Pathname();
        }
        else if (typeid(Dir) == typeid(file_entity)) {
            std::cout << file_entity.Pathname() + "/";
        }
        else if (typeid(OtherEntity) == typeid(file_entity)) {
            std::cout << file_entity.Pathname() + "(o1)";
        }
        else {
            assert(false);
        }
    }

    static void PrintPathname2(FileEntity const& file_entity)
    {
        if (typeid(File) == typeid(file_entity)) {
            std::cout << file_entity.Pathname();
        }
        else if (typeid(Dir) == typeid(file_entity)) {
            std::cout << find_files(file_entity.Pathname());
        }
        else if (typeid(OtherEntity) == typeid(file_entity)) {
            std::cout << file_entity.Pathname() + "(o2)";
        }
        else {
            assert(false);
        }
    }
};
// @@@ sample end

namespace {
TEST(Vistor, worst_pattern)
{
    auto file  = File{"visitor.cpp"};
    auto dir   = Dir{"find_files_ut_dir/dir0"};
    auto other = OtherEntity{"."};

    Printer::PrintPathname1(file);
    std::cout << std::endl;
    Printer::PrintPathname1(dir);
    std::cout << std::endl;
    Printer::PrintPathname1(other);
    std::cout << std::endl;

    Printer::PrintPathname2(file);
    std::cout << std::endl;
    Printer::PrintPathname2(dir);
    std::cout << std::endl;
    Printer::PrintPathname2(other);
    std::cout << std::endl;
}
}  // namespace
}  // namespace WorstPattern

namespace PolymorphicPattern {

// @@@ sample begin 1:0

class FileEntity {
public:
    explicit FileEntity(std::string pathname) : pathname_{std::move(pathname)} {}
    // @@@ ignore begin
    virtual ~FileEntity() {}
    std::string const& Pathname() const { return pathname_; }
    // @@@ ignore end
    virtual void PrintPathname1() const = 0;
    virtual void PrintPathname2() const = 0;

private:
    std::string const pathname_;
};

class File final : public FileEntity {
public:
    // @@@ ignore begin
    using FileEntity::FileEntity;
    // @@@ ignore end
    virtual void PrintPathname1() const override { std::cout << Pathname(); }
    virtual void PrintPathname2() const override { std::cout << Pathname(); }
};

class Dir final : public FileEntity {
public:
    // @@@ ignore begin
    using FileEntity::FileEntity;
    // @@@ ignore end
    virtual void PrintPathname1() const override { std::cout << Pathname() + "/"; }
    virtual void PrintPathname2() const override { std::cout << find_files(Pathname()); }
};

class OtherEntity final : public FileEntity {
public:
    // @@@ ignore begin
    using FileEntity::FileEntity;
    // @@@ ignore end
    virtual void PrintPathname1() const override { std::cout << Pathname() + "(o1)"; }
    virtual void PrintPathname2() const override { std::cout << Pathname() + "(o2)"; }
};

class Printer {
public:
    static void PrintPathname1(FileEntity const& file_entity) { file_entity.PrintPathname1(); }
    static void PrintPathname2(FileEntity const& file_entity) { file_entity.PrintPathname2(); }
};
// @@@ sample end

namespace {
TEST(Vistor, polymorphic_pattern)
{
    auto file  = File{"visitor.cpp"};
    auto dir   = Dir{"find_files_ut_dir/dir0"};
    auto other = OtherEntity{"."};

    Printer::PrintPathname1(file);
    std::cout << std::endl;
    Printer::PrintPathname1(dir);
    std::cout << std::endl;
    Printer::PrintPathname1(other);
    std::cout << std::endl;

    Printer::PrintPathname2(file);
    std::cout << std::endl;
    Printer::PrintPathname2(dir);
    std::cout << std::endl;
    Printer::PrintPathname2(other);
    std::cout << std::endl;
}
}  // namespace
}  // namespace PolymorphicPattern

namespace VisitorPattern {

// @@@ sample begin 2:0

void PathnamePrinter1::Visit(File const& file) { std::cout << file.Pathname(); }
void PathnamePrinter1::Visit(Dir const& dir) { std::cout << dir.Pathname() + "/"; }
void PathnamePrinter1::Visit(OtherEntity const& other) { std::cout << other.Pathname() + "(o1)"; }

void PathnamePrinter2::Visit(File const& file) { std::cout << file.Pathname(); }
void PathnamePrinter2::Visit(Dir const& dir) { std::cout << find_files(dir.Pathname()); }
void PathnamePrinter2::Visit(OtherEntity const& other) { std::cout << other.Pathname() + "(o2)"; }

class Printer {
public:
    static void PrintPathname1(FileEntity const& file_entity)
    {
        auto visitor = PathnamePrinter1{};

        file_entity.Accept(visitor);
    }

    static void PrintPathname2(FileEntity const& file_entity)
    {
        auto visitor = PathnamePrinter2{};

        file_entity.Accept(visitor);
    }
};
// @@@ sample end
// @@@ sample begin 3:0

void TestablePathnamePrinter1::Visit(File const& file) { ostream_ << file.Pathname(); }
void TestablePathnamePrinter1::Visit(Dir const& dir) { ostream_ << dir.Pathname() + "/"; }
void TestablePathnamePrinter1::Visit(OtherEntity const& other)
{
    ostream_ << other.Pathname() + "(o1)";
}

void TestablePathnamePrinter2::Visit(File const& file) { ostream_ << file.Pathname(); }

void TestablePathnamePrinter2::Visit(Dir const& dir) { ostream_ << find_files(dir.Pathname()); }

void TestablePathnamePrinter2::Visit(OtherEntity const& other)
{
    ostream_ << other.Pathname() + "(o2)";
}
// @@@ sample end
}  // namespace VisitorPattern
