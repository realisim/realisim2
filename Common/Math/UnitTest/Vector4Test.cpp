
#include "gtest/gtest.h"
#include "Math/Vector.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

TEST(Vector4, Constructor)
{
    {
        Vector4 v;
        EXPECT_EQ(v.x(), 0.0);
        EXPECT_EQ(v.y(), 0.0);
        EXPECT_EQ(v.z(), 0.0);
        EXPECT_EQ(v.w(), 0.0);
    }
    
    {
        Vector4 v(2.0);
        EXPECT_EQ(v.x(), 2.0);
        EXPECT_EQ(v.y(), 2.0);
        EXPECT_EQ(v.z(), 2.0);
        EXPECT_EQ(v.w(), 2.0);
    }
    
    {
        Vector4 v(1.0, 2.0, 3.0, 4.0);
        EXPECT_EQ(v.x(), 1.0);
        EXPECT_EQ(v.y(), 2.0);
        EXPECT_EQ(v.z(), 3.0);
        EXPECT_EQ(v.w(), 4.0);
    }
    
    {
        Vector4 v(1.0, 2.0, 3.0, 4.0);
        Vector4 v2(v);
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
        EXPECT_EQ(v2.z(), 3.0);
        EXPECT_EQ(v2.w(), 4.0);
    }
    
    {
        Vector4 v(1.0, 2.0, 3.0, 4.0);
        Vector4 v2 = v;
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
        EXPECT_EQ(v2.z(), 3.0);
        EXPECT_EQ(v2.w(), 4.0);
    }

    {
        Vector4 v( Vector2(1.0, 2.0), 3.0, 4.0);
        Vector4 v2 = v;
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
        EXPECT_EQ(v2.z(), 3.0);
        EXPECT_EQ(v2.w(), 4.0);
    }

    {
        Vector4 v( Vector3(1.0, 2.0, 3.0), 4.0);
        Vector4 v2 = v;
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
        EXPECT_EQ(v2.z(), 3.0);
        EXPECT_EQ(v2.w(), 4.0);
    }
}

TEST(Vector4, isEqual)
{
    Vector4 v1(-1.0, 1.0, 2.0, 4.0);
    bool e = v1.isEqual(v1);
    EXPECT_TRUE(e);
    
    Vector4 v2(-1.002, 1.0, 2.0, 4.0);
    e = v1.isEqual(v2);
    EXPECT_FALSE(e);
    
    //test epsilon
    e = v1.isEqual(v2, 0.02);
    EXPECT_TRUE(e);
    e = v1.isEqual(v2, 0.001);
    EXPECT_FALSE(e);
}

TEST(Vector4, normalization)
{
    Vector4 v;

    //verify it does not crah when dividing bt zero...
    v.norm();
    v.normalize();
    
    v = Vector4(1.0, 2.0, 3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.norm(), std::sqrt(30.0));
    
    //answer from wolfram alpha
    v.normalize();
    EXPECT_DOUBLE_EQ(v.norm(), 1.0);
    EXPECT_DOUBLE_EQ(v.x(), 1.0/std::sqrt(30));
    EXPECT_DOUBLE_EQ(v.y(), std::sqrt(2/15.0));
    EXPECT_DOUBLE_EQ(v.z(), std::sqrt(3/10.0));
    EXPECT_DOUBLE_EQ(v.w(), 2*std::sqrt(2/15.0));
}

TEST(Vector4, operator)
{
    Vector4 zero;
    Vector4 v(1.0, 2.0, 3.0, 4.0);
    
    //operator+
    {
        Vector4 v3 = zero + v;
        EXPECT_DOUBLE_EQ(v3.x(), 1.0);
        EXPECT_DOUBLE_EQ(v3.y(), 2.0);
        EXPECT_DOUBLE_EQ(v3.z(), 3.0);
        EXPECT_DOUBLE_EQ(v3.w(), 4.0);
    }
    
    //operator+=
    {
        Vector4 v1;
        v1 += v;
        EXPECT_DOUBLE_EQ(v1.x(), 1.0);
        EXPECT_DOUBLE_EQ(v1.y(), 2.0);
        EXPECT_DOUBLE_EQ(v1.z(), 3.0);
        EXPECT_DOUBLE_EQ(v1.w(), 4.0);
    }
    
    //operator- (const Vector4 &vect)
    {
        Vector4 v3 = zero - v;
        EXPECT_DOUBLE_EQ(v3.x(), -1.0);
        EXPECT_DOUBLE_EQ(v3.y(), -2.0);
        EXPECT_DOUBLE_EQ(v3.z(), -3.0);
        EXPECT_DOUBLE_EQ(v3.w(), -4.0);
    }
    
    //operator- ()
    {
        Vector4 v3 = -v;
        EXPECT_DOUBLE_EQ(v3.x(), -1.0);
        EXPECT_DOUBLE_EQ(v3.y(), -2.0);
        EXPECT_DOUBLE_EQ(v3.z(), -3.0);
        EXPECT_DOUBLE_EQ(v3.w(), -4.0);
    }
    
    //operator-= (const Vector4 &vect)
    {
        Vector4 v1;
        v1 -= v;
        EXPECT_DOUBLE_EQ(v1.x(), -1.0);
        EXPECT_DOUBLE_EQ(v1.y(), -2.0);
        EXPECT_DOUBLE_EQ(v1.z(), -3.0);
        EXPECT_DOUBLE_EQ(v1.w(), -4.0);
    }
    
    //operator* (double)
    {
        Vector4 v3 = v * 2;
        EXPECT_DOUBLE_EQ(v3.x(), 2.0);
        EXPECT_DOUBLE_EQ(v3.y(), 4.0);
        EXPECT_DOUBLE_EQ(v3.z(), 6.0);
        EXPECT_DOUBLE_EQ(v3.w(), 8.0);
        
        Vector4 v4 = 2*v;
        EXPECT_DOUBLE_EQ(v4.x(), 2.0);
        EXPECT_DOUBLE_EQ(v4.y(), 4.0);
        EXPECT_DOUBLE_EQ(v4.z(), 6.0);
        EXPECT_DOUBLE_EQ(v4.w(), 8.0);
    }
    
    //operator*= (double)
    {
        Vector4 v1(v);
        v1 *= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 2.0);
        EXPECT_DOUBLE_EQ(v1.y(), 4.0);
        EXPECT_DOUBLE_EQ(v1.z(), 6.0);
        EXPECT_DOUBLE_EQ(v1.w(), 8.0);
    }
    
    //operator/ (double)
    {
        Vector4 v3 = v / 2;
        EXPECT_DOUBLE_EQ(v3.x(), 0.5);
        EXPECT_DOUBLE_EQ(v3.y(), 1.0);
        EXPECT_DOUBLE_EQ(v3.z(), 1.5);
        EXPECT_DOUBLE_EQ(v3.w(), 2.0);
    }
    
    //operator/= (double)
    {
        Vector4 v1(v);
        v1 /= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 0.5);
        EXPECT_DOUBLE_EQ(v1.y(), 1.0);
        EXPECT_DOUBLE_EQ(v1.z(), 1.5);
        EXPECT_DOUBLE_EQ(v1.w(), 2.0);
    }
    
    //operator== (const Vector4&)
    {
        double e = v == Vector4(1.0, 2.0, 3.0, 4.0);
        EXPECT_TRUE(e);
    }
    
    //operator!= (const Vector4&)
    {
        double e = v == Vector4(1.0, 2.1, 3.0, 4.0);
        EXPECT_FALSE(e);
    }
    
    //double operator* (const Vector4 &vect) const; // dot product
    {
        Vector4 v3(5.0, 6.0, 7.0, 8.0);
        double d = v * v3;
        EXPECT_DOUBLE_EQ(d, 5 + 12 + 21 +32 );
    }
}

TEST(Vector4, Functions)
{
    {
        Vector4 v;
        v.setX(1.0);
        v.setY(2.0);
        v.setZ(3.0);
        v.setW(4.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
        EXPECT_DOUBLE_EQ(v.z(), 3.0);
        EXPECT_DOUBLE_EQ(v.w(), 4.0);
    }
    
    {
        Vector4 v;
        v.set(1.0, 2.0, 3.0, 4.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
        EXPECT_DOUBLE_EQ(v.z(), 3.0);
        EXPECT_DOUBLE_EQ(v.w(), 4.0);
    }
    
    {
        Vector4 v(1.0, 2.0, 3.0, 4.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
        EXPECT_DOUBLE_EQ(v.z(), 3.0);
        EXPECT_DOUBLE_EQ(v.w(), 4.0);
    }
    
    {
        Vector4 v(1.01, 2.02, 3.03, 4.04);
        std::string s = v.toString(1);
        EXPECT_STREQ(s.c_str(), "(1.0, 2.0, 3.0, 4.0)");
        
        s = v.toString(2);
        EXPECT_STREQ(s.c_str(), "(1.01, 2.02, 3.03, 4.04)");
    }
    
}