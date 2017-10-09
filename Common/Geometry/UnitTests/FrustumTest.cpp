#include "gtest/gtest.h"
#include "Geometry/Frustum.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;

TEST(Frustum, validation_ortho)
{
    //create a 20x20x20 ortho frustum...
    Frustum f;

    // -x
    Plane pLeft(Vector3(-10, 0, 0), Vector3(1, 0, 0));
    //x
    Plane pRight(Vector3(10, 0, 0), Vector3(-1, 0, 0));
    //y
    Plane pTop(Vector3  (0, 10, 0), Vector3(0, -1, 0));
    //-y
    Plane pBottom(Vector3(0, -10, 0), Vector3(0, 1, 0));
    //z
    Plane pNear(Vector3(0, 0, 10), Vector3(0, 0, -1));
    //-z
    Plane pFar(Vector3(0, 0, -10), Vector3(0, 0, 1));

    f.setPlane(Frustum::pnLeft, pLeft);
    f.setPlane(Frustum::pnRight, pRight);
    f.setPlane(Frustum::pnTop, pTop);
    f.setPlane(Frustum::pnBottom, pBottom);
    f.setPlane(Frustum::pnNear, pNear);
    f.setPlane(Frustum::pnFar, pFar);

    EXPECT_TRUE(f.contains(Vector3(0)));
    EXPECT_TRUE(f.contains(Vector3(5)));
    EXPECT_TRUE(f.contains(Vector3(-5)));
    EXPECT_TRUE(f.contains(Vector3(10)));
    
    EXPECT_FALSE(f.contains(Vector3(10), true));
    EXPECT_FALSE(f.contains(Vector3(12)));
}

