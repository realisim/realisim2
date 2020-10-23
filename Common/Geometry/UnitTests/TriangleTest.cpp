
#include <array>
#include "Core/Unused.h"
#include "gtest/gtest.h"

#include "Geometry/Intersections.h"
#include "Geometry/Line.h"
#include "Geometry/Triangle.h"
#include "Math/IsEqual.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

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

TEST(Triangle, lineIntersection)
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

TEST(Triangle, planeIntersection)
{
    Triangle t;

    // on xy plane
    t.set(Vector3(-1.0, -1.0, 0.0),
        Vector3(1.0, -1.0, 0.0),
        Vector3(0.0, 1.0, 0.0));

    IntersectionType iType;
    std::vector<Vector3> points;
    // z plane, coplanar with tri
    Plane zp(Vector3(0.0), Vector3(0.0, 0.0, 1.0));
    bool r = intersects(t, zp, &iType);
    EXPECT_TRUE(r);
    EXPECT_EQ(iType, itFullyContained);
    iType = intersect(t, zp, &points);
    EXPECT_EQ(iType, itFullyContained);

    // z plane, parallel with tri
    zp.set(Vector3(0.0, 0.0, 1.0), Vector3(0.0, 0.0, 1.0));
    r = intersects(t, zp, &iType);
    EXPECT_FALSE(r);
    EXPECT_EQ(iType, itNone);
    iType = intersect(t, zp, &points);
    EXPECT_EQ(iType, itNone);

    Plane yp(Vector3(0.0), Vector3(0.0, 1.0, 0.0));
    r = intersects(t, yp, &iType);
    EXPECT_TRUE(r);
    EXPECT_EQ(iType, itLineSegment);
    iType = intersect(t, yp, &points);
    EXPECT_EQ(iType, itLineSegment);
    EXPECT_DOUBLE_EQ(points[0].x(), 0.5);
    EXPECT_DOUBLE_EQ(points[0].y(), 0.0);
    EXPECT_DOUBLE_EQ(points[0].z(), 0.0);

    EXPECT_DOUBLE_EQ(points[1].x(), -0.5);
    EXPECT_DOUBLE_EQ(points[1].y(), 0.0);
    EXPECT_DOUBLE_EQ(points[1].z(), 0.0);
}

TEST(Triangle, aabbIntersection)
{
    Triangle t;
    // on xy plane
    t.set(Vector3(-10.0, -10.0, 0.0),
        Vector3(10.0, -10.0, 0.0),
        Vector3(0.0, 10.0, 0.0));

    IntersectionType iType = itNone;
    AxisAlignedBoundingBox aabb;

    // no intersection
    aabb.set(Vector3(30), Vector3(50));
    bool r = intersects(t, aabb, &iType);
    EXPECT_FALSE(r);
    EXPECT_EQ(iType, itNone);

    // triangle is outside, but AABB is crossed by the triangle plane
    // no intersection
    aabb.set(Vector3(30, 0, -10), Vector3(50, 20, 20));
    r = intersects(t, aabb, &iType);
    EXPECT_FALSE(r);
    EXPECT_EQ(iType, itNone);

    // fully contained
    aabb.set(Vector3(-30), Vector3(30));
    r = intersects(t, aabb, &iType);
    EXPECT_TRUE(r);
    EXPECT_EQ(iType, itFullyContained);

    // contains at least one vertex
    aabb.set(Vector3(-2), Vector3(30));
    r = intersects(t, aabb, &iType);
    EXPECT_TRUE(r);
    EXPECT_EQ(iType, itLineSegment);
}

//-----------------------------------------------------------------------------
// https://www.geogebra.org/m/ZuvmPjmy
//
TEST(Triangle, barycentricCoordinates)
{
    // on xy plane
    Triangle t;
    t.set(Vector3(-1.0, -1.0, 0.0),
        Vector3(1.0, -1.0, 0.0),
        Vector3(0.0, 1.0, 0.0));

    array<double, 3> c;
    Vector3 p;
    c = t.getBarycentricCoefficients(p);
    EXPECT_DOUBLE_EQ(0.25, c[0]);
    EXPECT_DOUBLE_EQ(0.25, c[1]);
    EXPECT_DOUBLE_EQ(0.5, c[2]);


    p.set(-1.0, 0.0, 0.0);
    c = t.getBarycentricCoefficients(p);
    EXPECT_DOUBLE_EQ( 0.75, c[0]);
    EXPECT_DOUBLE_EQ(-0.25, c[1]);
    EXPECT_DOUBLE_EQ( 0.5, c[2]);

    p.set(0.0, 3.0, 0.0);
    c = t.getBarycentricCoefficients(p);
    EXPECT_DOUBLE_EQ(-0.5, c[0]);
    EXPECT_DOUBLE_EQ(-0.5, c[1]);
    EXPECT_DOUBLE_EQ( 2.0, c[2]);


    p.set(2.0, 0.0, 0.0);
    c = t.getBarycentricCoefficients(p);
    EXPECT_DOUBLE_EQ(-0.75, c[0]);
    EXPECT_DOUBLE_EQ( 1.25, c[1]);
    EXPECT_DOUBLE_EQ( 0.5, c[2]);

    p.set(0.0, -1.0, 0.0);
    c = t.getBarycentricCoefficients(p);
    EXPECT_DOUBLE_EQ(0.5, c[0]);
    EXPECT_DOUBLE_EQ(0.5, c[1]);
    EXPECT_DOUBLE_EQ(0.0, c[2]);

    // out of plane...
    p.set(0.0, 0.0, 3.0);
    c = t.getBarycentricCoefficients(p);
    EXPECT_DOUBLE_EQ(-2.2767213258655068, c[0]);
    EXPECT_DOUBLE_EQ( 1.6955824957813170, c[1]);
    EXPECT_DOUBLE_EQ( 1.5811388300841898, c[2]);
}