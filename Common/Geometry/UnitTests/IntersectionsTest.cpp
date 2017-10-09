#include "gtest/gtest.h"
#include "Geometry/Intersections.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;

TEST(Intersections, linePlane)
{

    // 90 degrees
    {
        IntersectionType it0, it1;
        Line l0( Vector3(0.0, 0.0, 0.0),
                Vector3(0.0, 1.0, 0.0));
        
        Plane p0(Vector3(0.0, 0.0, 0.0),
                 Vector3(0.0, 1.0, 0.0));
        
        EXPECT_TRUE(intersects(l0, p0));
        EXPECT_TRUE(intersects(p0, l0));
        
        Vector3 r, r1;
        it0 = intersect(l0, p0, &r);
        it1 = intersect(p0, l0, &r1);
        EXPECT_EQ(r, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it0, itPoint);

        EXPECT_EQ(r1, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it1, itPoint);
    }
    
    // parallel no contact
    {
        IntersectionType it0, it1;
        Line l0( Vector3(0.0, 1.0, 0.0),
                Vector3(1.0, 1.0, 0.0));
        
        Plane p0(Vector3(0.0, 0.0, 0.0),
                 Vector3(0.0, 1.0, 0.0));
        
        EXPECT_FALSE(intersects(l0, p0));
        EXPECT_FALSE(intersects(p0, l0));
        
        Vector3 r, r1;
        it0 = intersect(l0, p0, &r);
        it1 = intersect(p0, l0, &r1);
        EXPECT_EQ(r, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it0, itNone);
        
        EXPECT_EQ(r1, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it1, itNone);
    }
    
    // parallel with contact
    {
        IntersectionType it0, it1;
        Line l0( Vector3(0.0, 0.0, 0.0),
                Vector3(1.0, 0.0, 0.0));
        
        Plane p0(Vector3(0.0, 0.0, 0.0),
                 Vector3(0.0, 1.0, 0.0));
        
        EXPECT_TRUE(intersects(l0, p0));
        EXPECT_TRUE(intersects(p0, l0));
        
        Vector3 r, r1;
        it0 = intersect(l0, p0, &r);
        it1 = intersect(p0, l0, &r1);
        EXPECT_EQ(r, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it0, itLine);
        
        EXPECT_EQ(r1, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it1, itLine);
    }
    
}