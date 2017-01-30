
#include "gtest/gtest.h"
#include "Math/Vector.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

TEST(Vector3, Constructor)
{
    {
        Vector3 v;
        EXPECT_EQ(v.x(), 0.0);
        EXPECT_EQ(v.y(), 0.0);
        EXPECT_EQ(v.z(), 0.0);
    }
    
    {
        Vector3 v(2.0);
        EXPECT_EQ(v.x(), 2.0);
        EXPECT_EQ(v.y(), 2.0);
        EXPECT_EQ(v.z(), 2.0);
    }
    
    {
        Vector3 v(1.0, 2.0, 3.0);
        EXPECT_EQ(v.x(), 1.0);
        EXPECT_EQ(v.y(), 2.0);
        EXPECT_EQ(v.z(), 3.0);
    }
    
    {
        Vector3 v(1.0, 2.0, 3.0);
        Vector3 v2(v);
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
        EXPECT_EQ(v2.z(), 3.0);
    }
    
    {
        Vector3 v(1.0, 2.0, 3.0);
        Vector3 v2 = v;
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
        EXPECT_EQ(v2.z(), 3.0);
    }

    {
        Vector3 v( Vector2(1.0, 2.0), 3.0);
        Vector3 v2 = v;
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
        EXPECT_EQ(v2.z(), 3.0);
    }
}

TEST(Vector3, isEqual)
{
    Vector3 v1(-1.0, 1.0, 2.0);
    bool e = v1.isEqual(v1);
    EXPECT_TRUE(e);
    
    Vector3 v2(-1.002, 1.0, 2.0);
    e = v1.isEqual(v2);
    EXPECT_FALSE(e);
    
    //test epsilon
    e = v1.isEqual(v2, 0.02);
    EXPECT_TRUE(e);
    e = v1.isEqual(v2, 0.001);
    EXPECT_FALSE(e);
}

TEST(Vector3, normalization)
{
    Vector3 v;

    //verify it does not crah when dividing bt zero...
    v.norm();
    v.normalize();
    
    v = Vector3(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.norm(), std::sqrt(14.0));
    
    v.normalize();
    EXPECT_DOUBLE_EQ(v.norm(), 1.0);
    EXPECT_DOUBLE_EQ(v.x(), 0.2672612419124244);
    EXPECT_DOUBLE_EQ(v.y(), 0.53452248382484879);
    EXPECT_DOUBLE_EQ(v.z(), 0.80178372573727319);
}

TEST(Vector3, operator)
{
    Vector3 zero;
    Vector3 v(1.0, 2.0, 3.0);
    
    //operator+
    {
        Vector3 v3 = zero + v;
        EXPECT_DOUBLE_EQ(v3.x(), 1.0);
        EXPECT_DOUBLE_EQ(v3.y(), 2.0);
        EXPECT_DOUBLE_EQ(v3.z(), 3.0);
    }
    
    //operator+=
    {
        Vector3 v1;
        v1 += v;
        EXPECT_DOUBLE_EQ(v1.x(), 1.0);
        EXPECT_DOUBLE_EQ(v1.y(), 2.0);
        EXPECT_DOUBLE_EQ(v1.z(), 3.0);
    }
    
    //operator- (const Vector3 &vect)
    {
        Vector3 v3 = zero - v;
        EXPECT_DOUBLE_EQ(v3.x(), -1.0);
        EXPECT_DOUBLE_EQ(v3.y(), -2.0);
        EXPECT_DOUBLE_EQ(v3.z(), -3.0);
    }
    
    //operator- ()
    {
        Vector3 v3 = -v;
        EXPECT_DOUBLE_EQ(v3.x(), -1.0);
        EXPECT_DOUBLE_EQ(v3.y(), -2.0);
        EXPECT_DOUBLE_EQ(v3.z(), -3.0);
    }
    
    //operator-= (const Vector3 &vect)
    {
        Vector3 v1;
        v1 -= v;
        EXPECT_DOUBLE_EQ(v1.x(), -1.0);
        EXPECT_DOUBLE_EQ(v1.y(), -2.0);
        EXPECT_DOUBLE_EQ(v1.z(), -3.0);
    }
    
    //operator* (double)
    {
        Vector3 v3 = v * 2;
        EXPECT_DOUBLE_EQ(v3.x(), 2.0);
        EXPECT_DOUBLE_EQ(v3.y(), 4.0);
        EXPECT_DOUBLE_EQ(v3.z(), 6.0);
        
        Vector3 v4 = 2*v;
        EXPECT_DOUBLE_EQ(v4.x(), 2.0);
        EXPECT_DOUBLE_EQ(v4.y(), 4.0);
        EXPECT_DOUBLE_EQ(v4.z(), 6.0);
    }
    
    //operator*= (double)
    {
        Vector3 v1(v);
        v1 *= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 2.0);
        EXPECT_DOUBLE_EQ(v1.y(), 4.0);
        EXPECT_DOUBLE_EQ(v1.z(), 6.0);
    }
    
    //operator/ (double)
    {
        Vector3 v3 = v / 2;
        EXPECT_DOUBLE_EQ(v3.x(), 0.5);
        EXPECT_DOUBLE_EQ(v3.y(), 1.0);
        EXPECT_DOUBLE_EQ(v3.z(), 1.5);
    }
    
    //operator/= (double)
    {
        Vector3 v1(v);
        v1 /= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 0.5);
        EXPECT_DOUBLE_EQ(v1.y(), 1.0);
        EXPECT_DOUBLE_EQ(v1.z(), 1.5);
    }
    
    //operator== (const Vector3&)
    {
        double e = v == Vector3(1.0, 2.0, 3.0);
        EXPECT_TRUE(e);
    }
    
    //operator!= (const Vector3&)
    {
        double e = v == Vector3(1.0, 2.1, 3.0);
        EXPECT_FALSE(e);
    }
    
    //double operator^ (const Vector3 &vect) const; // dot product
    {
        Vector3 v1(1, 2, 3);
        Vector3 v2(2, 3, 4);
        Vector3 r = v1 ^ v2;
        
        Vector3 expected(-1, 2, -1);
        EXPECT_TRUE( r.isEqual(expected, 1e-5) );
    }
    
    //double operator* (const Vector3 &vect) const; // dot product
    {
        Vector3 v3(4.0, 5.0, 6.0);
        double d = v * v3;
        EXPECT_DOUBLE_EQ(d, 4 + 10 + 18 );
    }
}

TEST(Vector3, Functions)
{
    {
        Vector3 v;
        v.setX(1.0);
        v.setY(2.0);
        v.setZ(3.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
        EXPECT_DOUBLE_EQ(v.z(), 3.0);
    }
    
    {
        Vector3 v;
        v.set(1.0, 2.0, 3.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
        EXPECT_DOUBLE_EQ(v.z(), 3.0);
    }
    
    {
        Vector3 v(1.0, 2.0, 3.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
        EXPECT_DOUBLE_EQ(v.z(), 3.0);
    }
    
    {
        Vector3 v(1.01, 2.02, 3.03);
        std::string s = v.toString(1);
        EXPECT_STREQ(s.c_str(), "(1.0, 2.0, 3.0)");
        
        s = v.toString(2);
        EXPECT_STREQ(s.c_str(), "(1.01, 2.02, 3.03)");
    }
    
}