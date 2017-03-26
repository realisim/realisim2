
#include "gtest/gtest.h"
#include "Math/Index.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

TEST(Index2, Constructor)
{
    {
        Index2 v;
        EXPECT_EQ(v.x(), 0);
        EXPECT_EQ(v.y(), 0);
    }
    
    {
        Index2 v(2);
        EXPECT_EQ(v.x(), 2);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Index2 v(1, 2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Index2 v(1, 2);
        Index2 v2(v);
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
    }
    
    {
        Index2 v(1, 2);
        Index2 v2 = v;
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
    }
}

TEST(Index2, normalization)
{
    Index2 v;    
    v = Index2(-1, 4);
    EXPECT_EQ(v.norm(), std::sqrt(17));
}

TEST(Index2, operator)
{
    Index2 zero;
    Index2 v(1, 2);
    
    //operator+
    {
        Index2 v3 = zero + v;
        EXPECT_EQ(v3.x(), 1);
        EXPECT_EQ(v3.y(), 2);
    }
    
    //operator+=
    {
        Index2 v1;
        v1 += v;
        EXPECT_EQ(v1.x(), 1);
        EXPECT_EQ(v1.y(), 2);
    }
    
    //operator- (const Index2 &vect)
    {
        Index2 v3 = zero - v;
        EXPECT_EQ(v3.x(), -1);
        EXPECT_EQ(v3.y(), -2);
    }
    
    //operator- ()
    {
        Index2 v3 = -v;
        EXPECT_EQ(v3.x(), -1);
        EXPECT_EQ(v3.y(), -2);
    }
    
    //operator-= (const Index2 &vect)
    {
        Index2 v1;
        v1 -= v;
        EXPECT_EQ(v1.x(), -1);
        EXPECT_EQ(v1.y(), -2);
    }
    
    //operator* (double)
    {
        Index2 v3 = v * 2;
        EXPECT_EQ(v3.x(), 2);
        EXPECT_EQ(v3.y(), 4);
        
        Index2 v4 = 2 * v;
        EXPECT_EQ(v4.x(), 2);
        EXPECT_EQ(v4.y(), 4);
    }
    
    //operator*= (double)
    {
        Index2 v1(v);
        v1 *= 2;
        EXPECT_EQ(v1.x(), 2);
        EXPECT_EQ(v1.y(), 4);
    }
    
    //operator/ (double)
    {
        Index2 v3 = v / 2;
        EXPECT_EQ(v3.x(), 0);
        EXPECT_EQ(v3.y(), 1);
    }
    
    //operator/= (double)
    {
        Index2 v1(v);
        v1 /= 2;
        EXPECT_EQ(v1.x(), 0);
        EXPECT_EQ(v1.y(), 1);
    }
    
    //operator== (const Index2&)
    {
        double e = v == Index2(1, 2);
        EXPECT_TRUE(e);
    }
    
    //operator!= (const Index2&)
    {
        double e = v == Index2(1, 3);
        EXPECT_FALSE(e);
    }
}

TEST(Index2, Functions)
{
    {
        Index2 v;
        v.setX(1);
        v.setY(2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Index2 v;
        v.set(1, 2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Index2 v(1, 2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Index2 v(11, 22);
        std::string s = v.toString();
        EXPECT_STREQ(s.c_str(), "(11, 22)");        
    }
    
}