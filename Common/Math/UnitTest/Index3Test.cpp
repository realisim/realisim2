
#include "gtest/gtest.h"
#include "Math/Index.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

TEST(Index3, Constructor)
{
    {
        Index3 v;
        EXPECT_EQ(v.x(), 0);
        EXPECT_EQ(v.y(), 0);
        EXPECT_EQ(v.z(), 0);
    }
    
    {
        Index3 v(2);
        EXPECT_EQ(v.x(), 2);
        EXPECT_EQ(v.y(), 2);
        EXPECT_EQ(v.z(), 2);
    }
    
    {
        Index3 v(1, 2, 3);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
        EXPECT_EQ(v.z(), 3);
    }
    
    {
        Index3 v(1, 2, 3);
        Index3 v2(v);
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
        EXPECT_EQ(v2.z(), 3);
    }
    
    {
        Index3 v(1, 2, 3);
        Index3 v2 = v;
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
        EXPECT_EQ(v2.z(), 3);
    }

    {
        Index3 v( Index2(1, 2), 3);
        Index3 v2 = v;
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
        EXPECT_EQ(v2.z(), 3);
    }
}

TEST(Index3, normalization)
{
    Index3 v;    
    v = Index3(1, 2, 3);
    EXPECT_DOUBLE_EQ(v.norm(), std::sqrt(14));
}

TEST(Index3, operator)
{
    Index3 zero;
    Index3 v(1, 2, 3);
    
    //operator+
    {
        Index3 v3 = zero + v;
        EXPECT_DOUBLE_EQ(v3.x(), 1);
        EXPECT_DOUBLE_EQ(v3.y(), 2);
        EXPECT_DOUBLE_EQ(v3.z(), 3);
    }
    
    //operator+=
    {
        Index3 v1;
        v1 += v;
        EXPECT_DOUBLE_EQ(v1.x(), 1);
        EXPECT_DOUBLE_EQ(v1.y(), 2);
        EXPECT_DOUBLE_EQ(v1.z(), 3);
    }
    
    //operator- (const Index3 &vect)
    {
        Index3 v3 = zero - v;
        EXPECT_DOUBLE_EQ(v3.x(), -1);
        EXPECT_DOUBLE_EQ(v3.y(), -2);
        EXPECT_DOUBLE_EQ(v3.z(), -3);
    }
    
    //operator- ()
    {
        Index3 v3 = -v;
        EXPECT_DOUBLE_EQ(v3.x(), -1);
        EXPECT_DOUBLE_EQ(v3.y(), -2);
        EXPECT_DOUBLE_EQ(v3.z(), -3);
    }
    
    //operator-= (const Index3 &vect)
    {
        Index3 v1;
        v1 -= v;
        EXPECT_DOUBLE_EQ(v1.x(), -1);
        EXPECT_DOUBLE_EQ(v1.y(), -2);
        EXPECT_DOUBLE_EQ(v1.z(), -3);
    }
    
    //operator* (double)
    {
        Index3 v3 = v * 2;
        EXPECT_DOUBLE_EQ(v3.x(), 2);
        EXPECT_DOUBLE_EQ(v3.y(), 4);
        EXPECT_DOUBLE_EQ(v3.z(), 6);
        
        Index3 v4 = 2*v;
        EXPECT_DOUBLE_EQ(v4.x(), 2);
        EXPECT_DOUBLE_EQ(v4.y(), 4);
        EXPECT_DOUBLE_EQ(v4.z(), 6);
    }
    
    //operator*= (double)
    {
        Index3 v1(v);
        v1 *= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 2);
        EXPECT_DOUBLE_EQ(v1.y(), 4);
        EXPECT_DOUBLE_EQ(v1.z(), 6);
    }
    
    //operator/ (double)
    {
        Index3 v3 = v / 2;
        EXPECT_DOUBLE_EQ(v3.x(), 0);
        EXPECT_DOUBLE_EQ(v3.y(), 1);
        EXPECT_DOUBLE_EQ(v3.z(), 1);
    }
    
    //operator/= (double)
    {
        Index3 v1(v);
        v1 /= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 0);
        EXPECT_DOUBLE_EQ(v1.y(), 1);
        EXPECT_DOUBLE_EQ(v1.z(), 1);
    }
    
    //operator== (const Index3&)
    {
        double e = v == Index3(1, 2, 3);
        EXPECT_TRUE(e);
    }
    
    //operator!= (const Index3&)
    {
        double e = v == Index3(1, 3, 3);
        EXPECT_FALSE(e);
    }
}

TEST(Index3, Functions)
{
    {
        Index3 v;
        v.setX(1);
        v.setY(2);
        v.setZ(3);
        EXPECT_DOUBLE_EQ(v.x(), 1);
        EXPECT_DOUBLE_EQ(v.y(), 2);
        EXPECT_DOUBLE_EQ(v.z(), 3);
    }
    
    {
        Index3 v;
        v.set(1, 2, 3);
        EXPECT_DOUBLE_EQ(v.x(), 1);
        EXPECT_DOUBLE_EQ(v.y(), 2);
        EXPECT_DOUBLE_EQ(v.z(), 3);
    }
    
    {
        Index3 v(1, 2, 3);
        EXPECT_DOUBLE_EQ(v.x(), 1);
        EXPECT_DOUBLE_EQ(v.y(), 2);
        EXPECT_DOUBLE_EQ(v.z(), 3);
    }
    
    {
        Index3 v(11, 22, 33);
        
        std::string s = v.toString();
        EXPECT_STREQ(s.c_str(), "(11, 22, 33)");
    }
    
}