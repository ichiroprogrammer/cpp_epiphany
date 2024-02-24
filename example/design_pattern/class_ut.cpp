#include "gtest_wrapper.h"

namespace {

namespace LangCpp {

// @@@ sample begin 0:0

class Point {
public:
    explicit Point(int x, int y) noexcept : x_{x}, y_{y} {}
    virtual ~Point() = default;

    void SetXY(int x, int y) noexcept
    {
        x_ = x;
        y_ = y;
    }

    void GetXY(int& x, int& y) const noexcept
    {
        x = x_;
        y = y_;
    }

    virtual int Quantity() const noexcept { return x_ * y_; }

    virtual void Multipy(int m) noexcept
    {
        x_ *= m;
        y_ *= m;
    }

private:
    int x_;
    int y_;
};
// @@@ sample end

TEST(Class, cpp_lang)
{
    // @@@ sample begin 0:1

    Point a{1, 2};

    int x;
    int y;
    a.GetXY(x, y);
    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);

    a.SetXY(3, 4);

    a.GetXY(x, y);
    ASSERT_EQ(x, 3);
    ASSERT_EQ(y, 4);

    ASSERT_EQ(a.Quantity(), 12);

    a.Multipy(2);
    ASSERT_EQ(a.Quantity(), 48);
    // @@@ sample end
}

// @@@ sample begin 1:0

class Point3D : public Point {
public:
    explicit Point3D(int x, int y, int z) noexcept : Point{x, y}, z_{z} {}

    void SetXYZ(int x, int y, int z) noexcept
    {
        SetXY(x, y);
        z_ = z;
    }

    void GetXYZ(int& x, int& y, int& z) const noexcept
    {
        GetXY(x, y);
        z = z_;
    }

    virtual int Quantity() const noexcept override { return Point::Quantity() * z_; }

    virtual void Multipy(int m) noexcept override
    {
        Point::Multipy(m);
        z_ *= m;
    }

private:
    int z_;
};
// @@@ sample end

TEST(Class, cpp_lang_override)
{
    // @@@ sample begin 1:1

    auto  a = Point3D{1, 2, 3};
    auto& b = a;

    auto x = int{};
    auto y = int{};
    b.GetXY(x, y);
    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);

    b.SetXY(3, 4);

    b.GetXY(x, y);
    ASSERT_EQ(x, 3);
    ASSERT_EQ(y, 4);

    ASSERT_EQ(b.Quantity(), 36);

    b.Multipy(2);
    ASSERT_EQ(b.Quantity(), 288);
    // @@@ sample end
}
}  // namespace LangCpp

namespace LangC {
// @@@ sample begin 2:0

struct Point {
    int x;
    int y;

    int (*const Quantity)(Point const* self);
    void (*const Multipy)(Point* self, int m);
};

static int point_quantity(Point const* self) { return self->x * self->y; }

static void point_multipy(Point* self, int m)
{
    self->x *= m;
    self->y *= m;
}

Point Point_Construct(int x, int y)
{
    Point ret = {x, y, point_quantity, point_multipy};  // C言語のつもり

    return ret;
}

void Point_SetXY(Point* self, int x, int y)
{
    self->x = x;
    self->y = y;
}

void Point_GetXY(Point* self, int* x, int* y)
{
    *x = self->x;
    *y = self->y;
}
// @@@ sample end

TEST(Class, c_lang)
{
    // @@@ sample begin 2:1

    Point a = Point_Construct(1, 2);

    int x;
    int y;

    Point_GetXY(&a, &x, &y);
    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);

    Point_SetXY(&a, 3, 4);

    Point_GetXY(&a, &x, &y);
    ASSERT_EQ(x, 3);
    ASSERT_EQ(y, 4);

    ASSERT_EQ(a.Quantity(&a), 12);

    a.Multipy(&a, 2);
    ASSERT_EQ(a.Quantity(&a), 48);
    // @@@ sample end
}

// @@@ sample begin 3:0

struct Point3D {
    Point point;
    int   z;
};

static int point3d_quantity(Point const* self)
{
    Point3D const* self_derived = (Point3D const*)self;

    return point_quantity(self) * self_derived->z;
}

static void point3d_multipy(Point* self, int m)
{
    point_multipy(self, m);

    Point3D* self_derived = (Point3D*)self;

    self_derived->z *= m;
}

Point3D Point3D_Construct(int x, int y, int z)
{
    Point3D ret{{x, y, point3d_quantity, point3d_multipy}, z};

    return ret;
}
// @@@ sample end

TEST(Class, c_lang_override)
{
    // @@@ sample begin 3:1

    Point3D a = Point3D_Construct(1, 2, 3);
    Point*  b = &a.point;

    int x;
    int y;

    Point_GetXY(b, &x, &y);
    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);

    Point_SetXY(b, 3, 4);

    Point_GetXY(b, &x, &y);
    ASSERT_EQ(x, 3);
    ASSERT_EQ(y, 4);

    ASSERT_EQ(b->Quantity(b), 36);

    b->Multipy(b, 2);
    ASSERT_EQ(b->Quantity(b), 288);
    // @@@ sample end
}
}  // namespace LangC
}  // namespace
