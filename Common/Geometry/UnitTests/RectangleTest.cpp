#include "gtest/gtest.h"
#include "Geometry/Rectangle.h"
#include "Math/IsEqual.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
   
// a macro is used so that gtest error reporting bt line works 
// as expected.
//
#define validateRectangle(iR, iBottomLeft, iWidth, iHeight) \
{ \
    const Vector2& bottomRight = iBottomLeft + Vector2(iWidth, 0.0); \
    const Vector2& topRight = iBottomLeft + Vector2(iWidth, iHeight); \
    const Vector2& topLeft = iBottomLeft + Vector2(0.0, iHeight); \
\
     EXPECT_TRUE(iR.getBottomLeft().isEqual(iBottomLeft)); \
    EXPECT_TRUE(iR.getBottomRight().isEqual(bottomRight)); \
    EXPECT_TRUE(iR.getTopRight().isEqual(topRight)); \
    EXPECT_TRUE(iR.getTopLeft().isEqual(topLeft)); \
\
     EXPECT_EQ(iR.getLeft(), iBottomLeft.x()); \
    EXPECT_EQ(iR.getRight(), topRight.x()); \
    EXPECT_EQ(iR.getBottom(), iBottomLeft.y()); \
    EXPECT_EQ(iR.getTop(), topRight.y()); \
\
    EXPECT_TRUE( iR.getSize().isEqual(Vector2(iWidth, iHeight), 1e-7)); \
    EXPECT_TRUE( Math::isEqual(iR.getWidth(), iWidth, 1e-7) ); \
    EXPECT_TRUE( Math::isEqual(iR.getHeight(), iHeight, 1e-7)); \
}

TEST(Rectangle, constructor)
{
    Rectangle r;
    EXPECT_FALSE(r.isValid());
    EXPECT_TRUE(r.getBottomLeft().isEqual(Vector2(0.0)));
    EXPECT_TRUE(r.getBottomRight().isEqual(Vector2(0.0)));
    EXPECT_TRUE(r.getTopRight().isEqual(Vector2(0.0)));
    EXPECT_TRUE(r.getTopLeft().isEqual(Vector2(0.0)));
    EXPECT_TRUE(r.getSize().isEqual(Vector2(0.0)));

    //bottomleft corner at 10,10; topRight at 20,30. so a 10x20 rect.
    Rectangle r1(Vector2(10, 10), Vector2(20, 30));
    EXPECT_TRUE(r1.isValid());
    validateRectangle(r1, Vector2(10, 10), 10.0, 20.0);

    // a 20x30 rectangle with bottomleft corner at -10,-10
    Rectangle r2(Vector2(-10, -10), 20, 30);
    EXPECT_TRUE(r2.isValid());
    validateRectangle(r2, Vector2(-10, -10), 20.0, 30.0);

    Rectangle r3(r2);
    EXPECT_TRUE(r3.isValid());
    validateRectangle(r3, Vector2(-10, -10), 20.0, 30.0);

    Rectangle r4;
    r4 = r2;
    EXPECT_TRUE(r4.isValid());
    validateRectangle(r4, Vector2(-10, -10), 20, 30);
}

TEST(Rectangle, set)
{
    Rectangle r;
    r.set(Vector2(10, 10), Vector2(20, 30));
    EXPECT_TRUE(r.isValid());
    validateRectangle(r, Vector2(10, 10), 10, 20);

    r.set(Vector2(-25, 12), 55, 45);
    EXPECT_TRUE(r.isValid());
    validateRectangle(r, Vector2(-25, 12), 55, 45);

    //bottom left 10,10 and size 10, 20
    Rectangle ref(Vector2(10, 10), Vector2(20, 30));
    r = ref;
    r.setBottom(-10);
    validateRectangle(r, Vector2(10, -10), 10, 40);

    r = ref;
    r.setTop(40);
    validateRectangle(r, Vector2(10, 10), 10, 30);

    r = ref;
    r.setLeft(-10);
    validateRectangle(r, Vector2(-10, 10), 30, 20);

    r = ref;
    r.setRight(50);
    validateRectangle(r, Vector2(10, 10), 40, 20);

    r = ref;
    r.setHeight(5);
    validateRectangle(r, Vector2(10, 10), 10, 5);

    r = ref;
    r.setWidth(5);
    validateRectangle(r, Vector2(10, 10), 5, 20);

    r = ref;
    r.setSize(Vector2(12.5, 17.9));
    validateRectangle(r, Vector2(10, 10), 12.5, 17.9);

    r = ref;
    r.setBottomLeft(Vector2(0.0, 0.0));
    validateRectangle(r, Vector2(0, 0), 20, 30);

    r = ref;
    r.setBottomRight(Vector2(15, 20));
    validateRectangle(r, Vector2(10, 20), 5, 10);

    r = ref;
    r.setTopRight(Vector2(30, 40));
    validateRectangle(r, Vector2(10, 10), 20, 30);

    r = ref;
    r.setTopLeft(Vector2(15, 15));
    validateRectangle(r, Vector2(15, 10), 5, 5);
}

TEST(Rectangle, intersection_and_union)
{
    Rectangle r1(Vector2(5, 10), 10, 15);
    Rectangle r2(Vector2(10, 5), 10, 10);
    Rectangle r3(Vector2(25, 20), 5, 5);

    EXPECT_TRUE(r1.intersects(r1));
    EXPECT_TRUE(r1.intersects(r2));
    EXPECT_FALSE(r1.intersects(r3));

    EXPECT_TRUE(r2.intersects(r1));
    EXPECT_TRUE(r2.intersects(r2));
    EXPECT_FALSE(r2.intersects(r3));

    EXPECT_FALSE(r3.intersects(r1));
    EXPECT_FALSE(r3.intersects(r2));
    EXPECT_TRUE(r3.intersects(r3));

    Rectangle i;
    i = r1.intersected(r1);
    validateRectangle(i, Vector2(5, 10), 10, 15);
    i = r1.intersected(r2);
    validateRectangle(i, Vector2(10, 10), 5, 5);
    i = r1.intersected(r3);
    EXPECT_FALSE(i.isValid());
    validateRectangle(i, Vector2(0.0, 0.0), 0.0, 0.0);

    i = r2.intersected(r1);
    validateRectangle(i, Vector2(10, 10), 5, 5);
    i = r2.intersected(r2);
    validateRectangle(i, Vector2(10, 5), 10, 10);
    i = r2.intersected(r3);
    EXPECT_FALSE(i.isValid());
    validateRectangle(i, Vector2(0.0, 0.0), 0.0, 0.0);

    i = r3.intersected(r1);
    EXPECT_FALSE(i.isValid());
    validateRectangle(i, Vector2(0.0, 0.0), 0.0, 0.0);
    i = r3.intersected(r2);
    EXPECT_FALSE(i.isValid());
    validateRectangle(i, Vector2(0.0, 0.0), 0.0, 0.0);
    i = r3.intersected(r3);
    EXPECT_TRUE(i.isValid());
    validateRectangle(i, Vector2(25, 20), 5, 5);

    //same test with some less round numbers...
    // r5 fully contained in r4
    Rectangle r4(Vector2(30, 5), 5, 10);
    Rectangle r5(Vector2(32.22, 7.44), 1, 1);
    i = r5.intersected(r4);
    validateRectangle(i, Vector2(32.22, 7.44), 1.0, 1.0);

    // not fully contained in r4
    Rectangle r6(Vector2(34.57, 7.44), 1, 1);
    i = r6.intersected(r4);
    validateRectangle(i, Vector2(34.57, 7.44), 0.43, 1);
}