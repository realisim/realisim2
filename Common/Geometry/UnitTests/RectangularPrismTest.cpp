#include "gtest/gtest.h"
#include "Geometry/RectangularPrism.h"
#include "Math/IsEqual.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
   
// a macro is used so that gtest error reporting bt line works 
// as expected.
//
#define validateRectangularPrism(iR, iBottomLeft, iWidth, iHeight, iDepth) \
{ \
    const Vector3& farBottomLeft = iBottomLeft; \
    const Vector3& farBottomRight = iBottomLeft + Vector3(iWidth, 0.0, 0.0); \
    const Vector3& farTopRight = farBottomRight + Vector3(0.0, iHeight, 0.0); \
    const Vector3& farTopLeft = iBottomLeft + Vector3(0.0, iHeight, 0.0); \
\
    const Vector3& nearBottomLeft = iBottomLeft + Vector3(0.0, 0.0, iDepth); \
    const Vector3& nearBottomRight = nearBottomLeft + Vector3(iWidth, 0.0, 0.0); \
    const Vector3& nearTopRight = nearBottomRight + Vector3(0.0, iHeight, 0.0); \
    const Vector3& nearTopLeft = nearBottomLeft + Vector3(0.0, iHeight, 0.0); \
\
    EXPECT_TRUE(iR.getNearBottomLeft().isEqual(nearBottomLeft)); \
    EXPECT_TRUE(iR.getNearBottomRight().isEqual(nearBottomRight)); \
    EXPECT_TRUE(iR.getNearTopRight().isEqual(nearTopRight)); \
    EXPECT_TRUE(iR.getNearTopLeft().isEqual(nearTopLeft)); \
\
    EXPECT_TRUE(iR.getFarBottomLeft().isEqual(farBottomLeft)); \
    EXPECT_TRUE(iR.getFarBottomRight().isEqual(farBottomRight)); \
    EXPECT_TRUE(iR.getFarTopRight().isEqual(farTopRight)); \
    EXPECT_TRUE(iR.getFarTopLeft().isEqual(farTopLeft)); \
\
    EXPECT_EQ(iR.getLeft(), nearBottomLeft.x()); \
    EXPECT_EQ(iR.getRight(), nearTopRight.x()); \
    EXPECT_EQ(iR.getBottom(), nearBottomLeft.y()); \
    EXPECT_EQ(iR.getTop(), nearTopRight.y()); \
\
    EXPECT_EQ(iR.getNear(), nearBottomLeft.z()); \
    EXPECT_EQ(iR.getFar(), farTopRight.z()); \
\
    EXPECT_TRUE( iR.getSize().isEqual(Vector3(iWidth, iHeight, iDepth), 1e-7)); \
    EXPECT_TRUE( Math::isEqual(iR.getWidth(), iWidth, 1e-7) ); \
    EXPECT_TRUE( Math::isEqual(iR.getHeight(), iHeight, 1e-7)); \
    EXPECT_TRUE( Math::isEqual(iR.getDepth(), iDepth, 1e-7)); \
}

TEST(RectangularPrism, constructor)
{
    RectangularPrism r;
    EXPECT_FALSE(r.isValid());
    EXPECT_TRUE(r.getNearBottomLeft().isEqual(Vector3(0.0)));
    EXPECT_TRUE(r.getNearBottomRight().isEqual(Vector3(0.0)));    
    EXPECT_TRUE(r.getNearTopRight().isEqual(Vector3(0.0)));
    EXPECT_TRUE(r.getNearTopLeft().isEqual(Vector3(0.0)));

    EXPECT_TRUE(r.getFarBottomLeft().isEqual(Vector3(0.0)));
    EXPECT_TRUE(r.getFarBottomRight().isEqual(Vector3(0.0)));
    EXPECT_TRUE(r.getFarTopRight().isEqual(Vector3(0.0)));
    EXPECT_TRUE(r.getFarTopLeft().isEqual(Vector3(0.0)));
    EXPECT_TRUE(r.getSize().isEqual(Vector3(0.0)));

    //bottomleft corner at 10,10; topRight at 20,30. so a 10x20x30 rect.
    RectangularPrism r1(Vector3(10, 10, 10), Vector3(20, 30, 40));
    EXPECT_TRUE(r1.isValid());
    validateRectangularPrism(r1, Vector3(10, 10, 10), 10.0, 20.0, 30);

    // a 20x30 rectangularPrism with bottomleft corner at -10,-10
    RectangularPrism r2(Vector3(-10, -10, -10), 20, 30, 40);
    EXPECT_TRUE(r2.isValid());
    validateRectangularPrism(r2, Vector3(-10, -10, -10), 20.0, 30.0, 40.0);

    RectangularPrism r3(r2);
    EXPECT_TRUE(r3.isValid());
    validateRectangularPrism(r3, Vector3(-10, -10, -10), 20.0, 30.0, 40);

    RectangularPrism r4;
    r4 = r2;
    EXPECT_TRUE(r4.isValid());
    validateRectangularPrism(r4, Vector3(-10, -10, -10), 20.0, 30.0, 40);
}

TEST(RectangularPrism, set)
{
    RectangularPrism r;
    r.set(Vector3(10, 10, 10), Vector3(20, 30, 40));
    EXPECT_TRUE(r.isValid());
    validateRectangularPrism(r, Vector3(10, 10, 10), 10, 20, 30);

    r.set(Vector3(-25, 12, -4), 55, 45, 22);
    EXPECT_TRUE(r.isValid());
    validateRectangularPrism(r, Vector3(-25, 12, -4), 55, 45, 22);

    //bottom left 10,10,10 and size 10, 20, 30
    RectangularPrism ref(Vector3(10, 10, 10), Vector3(20, 30, 40));
    r = ref;
    r.setBottom(-10);
    validateRectangularPrism(r, Vector3(10, -10, 10), 10, 40, 30);

    r = ref;
    r.setTop(40);
    validateRectangularPrism(r, Vector3(10, 10, 10), 10, 30, 30);

    r = ref;
    r.setLeft(-10);
    validateRectangularPrism(r, Vector3(-10, 10, 10), 30, 20, 30);

    r = ref;
    r.setRight(50);
    validateRectangularPrism(r, Vector3(10, 10, 10), 40, 20, 30);

    r = ref;
    r.setHeight(5);
    validateRectangularPrism(r, Vector3(10, 10, 10), 10, 5, 30);

    r = ref;
    r.setWidth(5);
    validateRectangularPrism(r, Vector3(10, 10, 10), 5, 20, 30);

    r = ref;
    r.setSize(Vector3(12.5, 17.9, 22.7));
    validateRectangularPrism(r, Vector3(10, 10, 10), 12.5, 17.9, 22.7);

    r = ref;
    r.setNearBottomLeft(Vector3(0.0, 0.0, 40.0));
    validateRectangularPrism(r, Vector3(0, 0, 10), 20, 30, 30);

    r = ref;
    r.setNearBottomRight(Vector3(15, 20, 25));
    validateRectangularPrism(r, Vector3(10, 20, 10), 5, 10, 15);

    r = ref;
    r.setNearTopRight(Vector3(20, 30, 35));
    validateRectangularPrism(r, Vector3(10, 10, 10), 10, 20, 25);

    r = ref;
    r.setNearTopLeft(Vector3(0, 30, 35));
    validateRectangularPrism(r, Vector3(0, 10, 10), 20, 20, 25);



    r = ref;
    r.setFarBottomLeft(Vector3(10.0, 10.0, 25.0));
    validateRectangularPrism(r, Vector3(10, 10, 25), 10, 20, 15);
}

TEST(RectangularPrism, intersection_and_union)
{
    RectangularPrism r1(Vector3(5, 10, 0), 10, 15, 20);
    RectangularPrism r2(Vector3(10, 5, 0), 10, 10, 10);
    RectangularPrism r3(Vector3(25, 20, 0), 5, 5, 5);

    EXPECT_TRUE(r1.intersects(r1));
    EXPECT_TRUE(r1.intersects(r2));
    EXPECT_FALSE(r1.intersects(r3));

    EXPECT_TRUE(r2.intersects(r1));
    EXPECT_TRUE(r2.intersects(r2));
    EXPECT_FALSE(r2.intersects(r3));

    EXPECT_FALSE(r3.intersects(r1));
    EXPECT_FALSE(r3.intersects(r2));
    EXPECT_TRUE(r3.intersects(r3));

    RectangularPrism i;
    i = r1.intersected(r1);
    validateRectangularPrism(i, Vector3(5, 10, 0), 10, 15, 20);
    i = r1.intersected(r2);
    validateRectangularPrism(i, Vector3(10, 10, 0), 5, 5, 10);
    i = r1.intersected(r3);
    EXPECT_FALSE(i.isValid());
    validateRectangularPrism(i, Vector3(0.0, 0.0, 0.0), 0.0, 0.0, 0.0);

    i = r2.intersected(r1);
    validateRectangularPrism(i, Vector3(10, 10, 0), 5, 5, 10);
    i = r2.intersected(r2);
    validateRectangularPrism(i, Vector3(10, 5, 0), 10, 10, 10);
    i = r2.intersected(r3);
    EXPECT_FALSE(i.isValid());
    validateRectangularPrism(i, Vector3(0.0, 0.0, 0.0), 0.0, 0.0, 0.0);

    i = r3.intersected(r1);
    EXPECT_FALSE(i.isValid());
    validateRectangularPrism(i, Vector3(0.0, 0.0, 0.0), 0.0, 0.0, 0.0);
    i = r3.intersected(r2);
    EXPECT_FALSE(i.isValid());
    validateRectangularPrism(i, Vector3(0.0, 0.0, 0.0), 0.0, 0.0, 0.0);
    i = r3.intersected(r3);
    EXPECT_TRUE(i.isValid());
    validateRectangularPrism(i, Vector3(25, 20, 0), 5, 5, 5);    
}