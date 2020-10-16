
#include "Core/Unused.h"
#include "gtest/gtest.h"

#include "Geometry/Intersections.h"
#include "Geometry/Line.h"
#include "Geometry/Triangle.h"
#include "Math/IsEqual.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;

TEST(Triangle, constructor)
{
    Triangle t;

    EXPECT_FALSE(t.isValid());
    
    t.set(Vector3(0.0, 0.0, 0.0),
        Vector3(1.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0));

    EXPECT_TRUE(t.isValid());
    EXPECT_EQ(t.getVertex(0), Vector3(0.0, 0.0, 0.0));
    EXPECT_EQ(t.getVertex(1), Vector3(1.0, 0.0, 0.0));
    EXPECT_EQ(t.getVertex(2), Vector3(0.0, 1.0, 0.0));

}

TEST(Triangle, functions)
{
    Triangle t;

    t.set(Vector3(0.0, 0.0, 0.0),
        Vector3(1.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0));

    double a = t.getArea();
    Vector3 c = t.getCentroid();
    
    EXPECT_FLOAT_EQ((float)a, 0.49999999f);

    UNUSED(c);

}

TEST(Triangle, intersection)
{
    Triangle t;
    t.set(Vector3(0.0, 0.0, 0.0),
        Vector3(1.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0));
   

    Line r;
    r.setOrigin( Vector3(0.0, 0.0, 5.0) );
    r.setDirection( Vector3(0.0, 0.0, -1.0) );

    Vector3 p, n;
    IntersectionType iType = intersect(r, t, &p, &n);
    UNUSED(iType);


    r.setOrigin(Vector3(-1.0, 0.0, 5.0));
    r.setDirection(Vector3(0.0, 0.0, -1.0));
    iType = intersect(r, t, &p, &n);
    UNUSED(iType);

    r.setOrigin(Vector3(0.5, 0.5, 5.0));
    r.setDirection(Vector3(0.0, 0.0, -1.0));
    iType = intersect(r, t, &p, &n);
    UNUSED(iType);

    r.setOrigin(Vector3(1.01, 0.0, 5.0));
    r.setDirection(Vector3(0.0, 0.0, -1.0));
    iType = intersect(r, t, &p, &n);
    UNUSED(iType);
}