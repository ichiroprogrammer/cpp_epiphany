#pragma once

namespace CRTP {

class File;
class Dir;
class OtherEntity;

class FileEntityVisitor {
public:
    virtual void Visit(File const&)        = 0;
    virtual void Visit(Dir const&)         = 0;
    virtual void Visit(OtherEntity const&) = 0;
    virtual ~FileEntityVisitor() {}
};

class PathnamePrinter1 final : public FileEntityVisitor {
public:
    virtual void Visit(File const&) override;
    virtual void Visit(Dir const&) override;
    virtual void Visit(OtherEntity const&) override;
};

class PathnamePrinter2 final : public FileEntityVisitor {
public:
    virtual void Visit(File const&) override;
    virtual void Visit(Dir const&) override;
    virtual void Visit(OtherEntity const&) override;
};

// @@@ sample begin 0:0

class FileEntity {  // VisitorのFileEntityと同じ
public:
    explicit FileEntity(std::string&& pathname) : pathname_{std::move(pathname)} {}
    virtual ~FileEntity() {}
    std::string const& Pathname() const { return pathname_; }

    virtual void Accept(FileEntityVisitor&) const = 0;  // Acceptの仕様は安定しているので
                                                        // NVIは使わない。
private:
    std::string const pathname_;
};

template <typename T>
class AcceptableFileEntity : public FileEntity {  // CRTP
public:
    virtual void Accept(FileEntityVisitor& visitor) const override
    {
        visitor.Visit(*static_cast<T const*>(this));
    }

private:
    // T : public AcceptableFileEntity<T> { ... };
    // 以外の使い方をコンパイルエラーにする
    AcceptableFileEntity(std::string&& pathname) : FileEntity{std::move(pathname)} {}
    friend T;
};

class File final : public AcceptableFileEntity<File> {  // CRTPでクローンを解消
public:
    File(std::string pathname) : AcceptableFileEntity{std::move(pathname)} {}
};

class Dir final : public AcceptableFileEntity<Dir> {  // CRTPでクローンを解消
public:
    Dir(std::string pathname) : AcceptableFileEntity{std::move(pathname)} {}
};

class OtherEntity final : public AcceptableFileEntity<OtherEntity> {  // CRTPでクローンを解消
public:
    OtherEntity(std::string pathname) : AcceptableFileEntity{std::move(pathname)} {}
};
// @@@ sample end

class TestablePrinter : public FileEntityVisitor {
public:
    explicit TestablePrinter(std::ostream& os) : ostream_{os} {}

protected:
    std::ostream& ostream_;
};

class TestablePathnamePrinter1 final : public TestablePrinter {
public:
    explicit TestablePathnamePrinter1(std::ostream& os) : TestablePrinter{os} {}
    virtual void Visit(File const& file) override;
    virtual void Visit(Dir const& dir) override;
    virtual void Visit(OtherEntity const& other) override;
};

class TestablePathnamePrinter2 final : public TestablePrinter {
public:
    explicit TestablePathnamePrinter2(std::ostream& os) : TestablePrinter{os} {}
    virtual void Visit(File const& file) override;
    virtual void Visit(Dir const& dir) override;
    virtual void Visit(OtherEntity const& other) override;
};
}  // namespace CRTP
