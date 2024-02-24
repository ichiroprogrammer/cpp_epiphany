#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <typeinfo>

#include "crtp.h"

namespace {
std::string find_files(std::string const& dir)
{
    namespace fs = std::filesystem;  // OK 長い名前を短く

    auto files = std::vector<std::string>{};
    auto top   = fs::path{dir};

    std::for_each(fs::recursive_directory_iterator{top}, fs::recursive_directory_iterator{},
                  [&](fs::path const& p) { files.emplace_back(p.generic_string()); });

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

namespace CRTP {

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
}  // namespace CRTP
