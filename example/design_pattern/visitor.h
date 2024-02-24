#pragma once

namespace VisitorPattern {

class File;
class Dir;
class OtherEntity;

// @@@ sample begin 0:0

class FileEntityVisitor {
public:
    virtual void Visit(File const&)        = 0;
    virtual void Visit(Dir const&)         = 0;
    virtual void Visit(OtherEntity const&) = 0;
    // @@@ ignore begin
    virtual ~FileEntityVisitor() {}
    // @@@ ignore end
};

class FileEntity {
public:
    explicit FileEntity(std::string pathname) : pathname_{std::move(pathname)} {}
    // @@@ ignore begin
    virtual ~FileEntity() {}
    // @@@ ignore end
    std::string const& Pathname() const { return pathname_; }

    virtual void Accept(FileEntityVisitor&) const = 0;  // Acceptの仕様は安定しているので
                                                        // NVIは使わない。
private:
    std::string const pathname_;
};

class File final : public FileEntity {
public:
    using FileEntity::FileEntity;
    // @@@ sample end
    // @@@ sample begin 0:1
    virtual void Accept(FileEntityVisitor& visitor) const override { visitor.Visit(*this); }
    // @@@ sample end
    // @@@ sample begin 0:2
};

class Dir final : public FileEntity {
public:
    using FileEntity::FileEntity;
    virtual void Accept(FileEntityVisitor& visitor) const override { visitor.Visit(*this); }
};

class OtherEntity final : public FileEntity {
public:
    using FileEntity::FileEntity;
    virtual void Accept(FileEntityVisitor& visitor) const override { visitor.Visit(*this); }
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
// @@@ sample end

// @@@ sample begin 1:0

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
// @@@ sample end
}  // namespace VisitorPattern
