
#include "gtest/gtest.h"
#include "Math/VectorI.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

TEST(Vector2i, Constructor)
{
    {
        Vector2i v;
        EXPECT_EQ(v.x(), 0);
        EXPECT_EQ(v.y(), 0);
    }
    
    {
        Vector2i v(2);
        EXPECT_EQ(v.x(), 2);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Vector2i v(1, 2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Vector2i v(1, 2);
        Vector2i v2(v);
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
    }
    
    {
        Vector2i v(1, 2);
        Vector2i v2 = v;
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
    }
}

TEST(Vector2i, normalization)
{
    Vector2i v;    
    v = Vector2i(-1, 4);
    EXPECT_EQ(v.norm(), std::sqrt(17));

    EXPECT_EQ(v.normSquare(), 17);
}

TEST(Vector2i, operator)
{
    Vector2i zero;
    Vector2i v(1, 2);
    
    //operator+
    {
        Vector2i v3 = zero + v;
        EXPECT_EQ(v3.x(), 1);
        EXPECT_EQ(v3.y(), 2);
    }
    
    //operator+=
    {
        Vector2i v1;
        v1 += v;
        EXPECT_EQ(v1.x(), 1);
        EXPECT_EQ(v1.y(), 2);
    }
    
    //operator- (const Vector2i &vect)
    {
        Vector2i v3 = zero - v;
        EXPECT_EQ(v3.x(), -1);
        EXPECT_EQ(v3.y(), -2);
    }
    
    //operator- ()
    {
        Vector2i v3 = -v;
        EXPECT_EQ(v3.x(), -1);
        EXPECT_EQ(v3.y(), -2);
    }
    
    //operator-= (const Vector2i &vect)
    {
        Vector2i v1;
        v1 -= v;
        EXPECT_EQ(v1.x(), -1);
        EXPECT_EQ(v1.y(), -2);
    }
    
    //operator* (double)
    {
        Vector2i v3 = v * 2;
        EXPECT_EQ(v3.x(), 2);
        EXPECT_EQ(v3.y(), 4);
        
        Vector2i v4 = 2 * v;
        EXPECT_EQ(v4.x(), 2);
        EXPECT_EQ(v4.y(), 4);
    }
    
    //operator*= (double)
    {
        Vector2i v1(v);
        v1 *= 2;
        EXPECT_EQ(v1.x(), 2);
        EXPECT_EQ(v1.y(), 4);
    }
    
    //operator/ (double)
    {
        Vector2i v3 = v / 2;
        EXPECT_EQ(v3.x(), 0);
        EXPECT_EQ(v3.y(), 1);
    }
    
    //operator/= (double)
    {
        Vector2i v1(v);
        v1 /= 2;
        EXPECT_EQ(v1.x(), 0);
        EXPECT_EQ(v1.y(), 1);
    }
    
    //operator== (const Vector2i&)
    {
        double e = v == Vector2i(1, 2);
        EXPECT_TRUE(e);
    }
    
    //operator!= (const Vector2i&)
    {
        double e = v == Vector2i(1, 3);
        EXPECT_FALSE(e);
    }

    //bool operator > (const Vector2 &vect) const;
    //bool operator >= (const Vector2 &vect) const;
    //bool operator < (const Vector2 &vect) const;
    //bool operator <= (const Vector2 &vect) const;
    {
        Vector2i ref(4, 5);

        Vector2i v1(5, 5);
        Vector2i v2(4, 6);
        Vector2i v3(5, 6);

        Vector2i v4(3, 5);
        Vector2i v5(4, 4);
        Vector2i v6(3, 4);


        //operator>
        EXPECT_FALSE(ref > ref);
        EXPECT_FALSE(v1 > ref);
        EXPECT_FALSE(v2 > ref);
        EXPECT_TRUE(v3 > ref);

        EXPECT_FALSE(v4 > ref);
        EXPECT_FALSE(v5 > ref);
        EXPECT_FALSE(v6 > ref);

        //operator>=
        EXPECT_TRUE(ref >= ref);
        EXPECT_TRUE(v1 >= ref);
        EXPECT_TRUE(v2 >= ref);
        EXPECT_TRUE(v3 >= ref);

        EXPECT_FALSE(v4 >= ref);
        EXPECT_FALSE(v5 >= ref);
        EXPECT_FALSE(v6 >= ref);

        //operator<
        EXPECT_FALSE(ref < ref);
        EXPECT_FALSE(v1 < ref);
        EXPECT_FALSE(v2 < ref);
        EXPECT_FALSE(v3 < ref);

        EXPECT_FALSE(v4 < ref);
        EXPECT_FALSE(v5 < ref);
        EXPECT_TRUE(v6 < ref);

        //operator<=
        EXPECT_TRUE(ref <= ref);
        EXPECT_FALSE(v1 <= ref);
        EXPECT_FALSE(v2 <= ref);
        EXPECT_FALSE(v3 <= ref);

        EXPECT_TRUE(v4 <= ref);
        EXPECT_TRUE(v5 <= ref);
        EXPECT_TRUE(v6 <= ref);
    }
}

TEST(Vector2i, Functions)
{
    {
        Vector2i v;
        v.setX(1);
        v.setY(2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Vector2i v;
        v.set(1, 2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Vector2i v(1, 2);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
    }
    
    {
        Vector2i v(11, 22);
        std::string s = v.toString();
        EXPECT_STREQ(s.c_str(), "(11, 22)");        
    }
    
}