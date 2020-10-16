#include "gtest/gtest.h"
#include "Geometry/Sphere.h"
#include "Math/IsEqual.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;

TEST(Sphere, constructor)
{
    Sphere s;

    EXPECT_TRUE(s.isValid());
    EXPECT_EQ(s.getCenter(), Vector3());
    EXPECT_EQ(s.getRadius(), 0.0);
}

TEST(Sphere, functions)
{
    Sphere s;
    Vector3 center(10, 11, 12);

    s.setCenter(center);
    s.setRadius(12);

    EXPECT_TRUE(s.isValid());
    EXPECT_EQ(s.getCenter(), center);
    EXPECT_EQ(s.getRadius(), 12.0);
    EXPECT_EQ(s.getArea(), 1809.5573684677208);
    EXPECT_EQ(s.getVolume(), 7238.2294738708833);

    EXPECT_TRUE(s.contains(Vector3(8.0, 9.0, 10.0)));
    EXPECT_FALSE(s.contains(Vector3(-3, 0.0, 0.0)));
}