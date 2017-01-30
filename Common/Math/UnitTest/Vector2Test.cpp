
#include "gtest/gtest.h"
#include "Math/Vector.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

TEST(Vector2, Constructor)
{
    {
        Vector2 v;
        EXPECT_EQ(v.x(), 0.0);
        EXPECT_EQ(v.y(), 0.0);
    }
    
    {
        Vector2 v(2.0);
        EXPECT_EQ(v.x(), 2.0);
        EXPECT_EQ(v.y(), 2.0);
    }
    
    {
        Vector2 v(1.0, 2.0);
        EXPECT_EQ(v.x(), 1.0);
        EXPECT_EQ(v.y(), 2.0);
    }
    
    {
        Vector2 v(1.0, 2.0);
        Vector2 v2(v);
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
    }
    
    {
        Vector2 v(1.0, 2.0);
        Vector2 v2 = v;
        EXPECT_EQ(v2.x(), 1.0);
        EXPECT_EQ(v2.y(), 2.0);
    }
}

TEST(Vector2, isEqual)
{
    Vector2 v1(-1.0, 1.0);
    bool e = v1.isEqual(v1);
    EXPECT_TRUE(e);
    
    Vector2 v2(-1.002, 1.0);
    e = v1.isEqual(v2);
    EXPECT_FALSE(e);
    
    //test epsilon
    e = v1.isEqual(v2, 0.02);
    EXPECT_TRUE(e);
    e = v1.isEqual(v2, 0.001);
    EXPECT_FALSE(e);
}

TEST(Vector2, normalization)
{
    Vector2 v;
    v.norm();
    v.normalize();
    
    v = Vector2(-1.0, 4.0);
    EXPECT_DOUBLE_EQ(v.norm(), std::sqrt(17.0));
    
    v.normalize();
    EXPECT_DOUBLE_EQ(v.norm(), 1.0);
    EXPECT_DOUBLE_EQ(v.x(), -0.24253562503633297);
    EXPECT_DOUBLE_EQ(v.y(), 0.97014250014533187);
}

TEST(Vector2, operator)
{
    Vector2 zero;
    Vector2 v(1.0, 2.0);
    
    //operator+
    {
        Vector2 v3 = zero + v;
        EXPECT_DOUBLE_EQ(v3.x(), 1.0);
        EXPECT_DOUBLE_EQ(v3.y(), 2.0);
    }
    
    //operator+=
    {
        Vector2 v1;
        v1 += v;
        EXPECT_DOUBLE_EQ(v1.x(), 1.0);
        EXPECT_DOUBLE_EQ(v1.y(), 2.0);
    }
    
    //operator- (const Vector2 &vect)
    {
        Vector2 v3 = zero - v;
        EXPECT_DOUBLE_EQ(v3.x(), -1.0);
        EXPECT_DOUBLE_EQ(v3.y(), -2.0);
    }
    
    //operator- ()
    {
        Vector2 v3 = -v;
        EXPECT_DOUBLE_EQ(v3.x(), -1.0);
        EXPECT_DOUBLE_EQ(v3.y(), -2.0);
    }
    
    //operator-= (const Vector2 &vect)
    {
        Vector2 v1;
        v1 -= v;
        EXPECT_DOUBLE_EQ(v1.x(), -1.0);
        EXPECT_DOUBLE_EQ(v1.y(), -2.0);
    }
    
    //operator* (double)
    {
        Vector2 v3 = v * 2;
        EXPECT_DOUBLE_EQ(v3.x(), 2.0);
        EXPECT_DOUBLE_EQ(v3.y(), 4.0);
        
        Vector2 v4 = 2 * v;
        EXPECT_DOUBLE_EQ(v4.x(), 2.0);
        EXPECT_DOUBLE_EQ(v4.y(), 4.0);
    }
    
    //operator*= (double)
    {
        Vector2 v1(v);
        v1 *= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 2.0);
        EXPECT_DOUBLE_EQ(v1.y(), 4.0);
    }
    
    //operator/ (double)
    {
        Vector2 v3 = v / 2;
        EXPECT_DOUBLE_EQ(v3.x(), 0.5);
        EXPECT_DOUBLE_EQ(v3.y(), 1.0);
    }
    
    //operator/= (double)
    {
        Vector2 v1(v);
        v1 /= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 0.5);
        EXPECT_DOUBLE_EQ(v1.y(), 1.0);
    }
    
    //operator== (const Vector2&)
    {
        double e = v == Vector2(1.0, 2.0);
        EXPECT_TRUE(e);
    }
    
    //operator!= (const Vector2&)
    {
        double e = v == Vector2(1.0, 2.1);
        EXPECT_FALSE(e);
    }
    
    //double operator* (const Vector2 &vect) const; // dot product
    {
        Vector2 v3(4.0, 5.0);
        double d = v * v3;
        EXPECT_DOUBLE_EQ(d, 4 + 10 );
    }
}

TEST(Vector2, Functions)
{
    {
        Vector2 v;
        v.setX(1.0);
        v.setY(2.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
    }
    
    {
        Vector2 v;
        v.set(1.0, 2.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
    }
    
    {
        Vector2 v(1.0, 2.0);
        EXPECT_DOUBLE_EQ(v.x(), 1.0);
        EXPECT_DOUBLE_EQ(v.y(), 2.0);
    }
    
    {
        Vector2 v(1.01, 2.02);
        std::string s = v.toString(1);
        EXPECT_STREQ(s.c_str(), "(1.0, 2.0)");
        
        s = v.toString(2);
        EXPECT_STREQ(s.c_str(), "(1.01, 2.02)");
    }
    
}