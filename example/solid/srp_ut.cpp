#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "gtest_wrapper.h"

namespace {

class SRP : public ::testing::Test {
protected:
    virtual void SetUp() noexcept override { remove_file(not_srp_text_); }

    virtual void TearDown() noexcept override { remove_file(not_srp_text_); }

    std::string const not_srp_text_ = "not_srp.txt";

    static void remove_file(std::string const& filename) noexcept
    {
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }
};

// @@@ sample begin 0:0

class SentenceHolderNotSRP {
public:
    SentenceHolderNotSRP()  = default;
    ~SentenceHolderNotSRP() = default;

    void Add(std::string const& sentence) { sentence_ += sentence; }

    std::string const& Get() const noexcept { return sentence_; }

    void Save(std::string const& file)
    {
        std::ofstream o{file};
        o << sentence_;
    }

    void Display() { std::cout << sentence_; }

private:
    std::string sentence_{};
};
// @@@ sample end

TEST_F(SRP, srp_violation)
{
    // @@@ sample begin 0:1

    auto not_srp = SentenceHolderNotSRP{};

    not_srp.Add("haha\n");
    not_srp.Add("hihi\n");
    not_srp.Add("huhu\n");

    // SRPに従っていないため、テストが面倒
    not_srp.Save(not_srp_text_);  // not_srp_text_への書き込み

    auto ifs       = std::ifstream{not_srp_text_};
    auto ifs_begin = std::istreambuf_iterator<char>{ifs};
    auto ifs_end   = std::istreambuf_iterator<char>{};
    auto act       = std::string{ifs_begin, ifs_end};  // not_srp_text_ファイルの読み出し

    ASSERT_EQ("haha\nhihi\nhuhu\n", act);

    // SRPに従っていないため、テストできない
    not_srp.Display();
    // @@@ sample end
}
// @@@ sample begin 1:0

class SentenceHolderSRP {
public:
    SentenceHolderSRP()  = default;
    ~SentenceHolderSRP() = default;

    void Add(std::string const& sentence) { sentence_ += sentence; }

    std::string const& Get() const noexcept { return sentence_; }

private:
    std::string sentence_{};
};

// SRPに従うために、
//    SentenceHolderNotSRP::Save(), SentenceHolderNotSRP::Display()
// をクラスの外に出し、さらに仮引数に出力先(std::ostream&)を追加してこの2関数を統一。
void Output(SentenceHolderSRP const& sentence_holder, std::ostream& o)
{
    o << sentence_holder.Get();
}
// @@@ sample end

TEST_F(SRP, srp_no_violation)
{
    // @@@ sample begin 1:1

    auto srp = SentenceHolderSRP{};

    srp.Add("haha\n");
    srp.Add("hihi\n");
    srp.Add("huhu\n");

    // SRPに従ったことで、ファイル操作やstd::coutへの操作が不要になり、単体テストの実施が容易
    auto act = std::ostringstream{};
    Output(srp, act);

    ASSERT_EQ("haha\nhihi\nhuhu\n", act.str());
    // @@@ sample end
}
}  // namespace
