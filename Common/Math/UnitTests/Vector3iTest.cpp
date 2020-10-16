
#include "gtest/gtest.h"
#include "Math/VectorI.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

TEST(Vector3i, Constructor)
{
    {
        Vector3i v;
        EXPECT_EQ(v.x(), 0);
        EXPECT_EQ(v.y(), 0);
        EXPECT_EQ(v.z(), 0);
    }
    
    {
        Vector3i v(2);
        EXPECT_EQ(v.x(), 2);
        EXPECT_EQ(v.y(), 2);
        EXPECT_EQ(v.z(), 2);
    }
    
    {
        Vector3i v(1, 2, 3);
        EXPECT_EQ(v.x(), 1);
        EXPECT_EQ(v.y(), 2);
        EXPECT_EQ(v.z(), 3);
    }
    
    {
        Vector3i v(1, 2, 3);
        Vector3i v2(v);
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
        EXPECT_EQ(v2.z(), 3);
    }
    
    {
        Vector3i v(1, 2, 3);
        Vector3i v2 = v;
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
        EXPECT_EQ(v2.z(), 3);
    }

    {
        Vector3i v( Vector2i(1, 2), 3);
        Vector3i v2 = v;
        EXPECT_EQ(v2.x(), 1);
        EXPECT_EQ(v2.y(), 2);
        EXPECT_EQ(v2.z(), 3);
    }
}

TEST(Vector3i, normalization)
{
    Vector3i v;    
    v = Vector3i(1, 2, 3);
    EXPECT_DOUBLE_EQ(v.norm(), std::sqrt(14));

    EXPECT_DOUBLE_EQ(v.normSquared(), 14);
}

TEST(Vector3i, operator)
{
    Vector3i zero;
    Vector3i v(1, 2, 3);
    
    //operator+
    {
        Vector3i v3 = zero + v;
        EXPECT_DOUBLE_EQ(v3.x(), 1);
        EXPECT_DOUBLE_EQ(v3.y(), 2);
        EXPECT_DOUBLE_EQ(v3.z(), 3);
    }
    
    //operator+=
    {
        Vector3i v1;
        v1 += v;
        EXPECT_DOUBLE_EQ(v1.x(), 1);
        EXPECT_DOUBLE_EQ(v1.y(), 2);
        EXPECT_DOUBLE_EQ(v1.z(), 3);
    }
    
    //operator- (const Vector3i &vect)
    {
        Vector3i v3 = zero - v;
        EXPECT_DOUBLE_EQ(v3.x(), -1);
        EXPECT_DOUBLE_EQ(v3.y(), -2);
        EXPECT_DOUBLE_EQ(v3.z(), -3);
    }
    
    //operator- ()
    {
        Vector3i v3 = -v;
        EXPECT_DOUBLE_EQ(v3.x(), -1);
        EXPECT_DOUBLE_EQ(v3.y(), -2);
        EXPECT_DOUBLE_EQ(v3.z(), -3);
    }
    
    //operator-= (const Vector3i &vect)
    {
        Vector3i v1;
        v1 -= v;
        EXPECT_DOUBLE_EQ(v1.x(), -1);
        EXPECT_DOUBLE_EQ(v1.y(), -2);
        EXPECT_DOUBLE_EQ(v1.z(), -3);
    }
    
    //operator* (double)
    {
        Vector3i v3 = v * 2;
        EXPECT_DOUBLE_EQ(v3.x(), 2);
        EXPECT_DOUBLE_EQ(v3.y(), 4);
        EXPECT_DOUBLE_EQ(v3.z(), 6);
        
        Vector3i v4 = 2*v;
        EXPECT_DOUBLE_EQ(v4.x(), 2);
        EXPECT_DOUBLE_EQ(v4.y(), 4);
        EXPECT_DOUBLE_EQ(v4.z(), 6);
    }
    
    //operator*= (double)
    {
        Vector3i v1(v);
        v1 *= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 2);
        EXPECT_DOUBLE_EQ(v1.y(), 4);
        EXPECT_DOUBLE_EQ(v1.z(), 6);
    }
    
    //operator/ (double)
    {
        Vector3i v3 = v / 2;
        EXPECT_DOUBLE_EQ(v3.x(), 0);
        EXPECT_DOUBLE_EQ(v3.y(), 1);
        EXPECT_DOUBLE_EQ(v3.z(), 1);
    }
    
    //operator/= (double)
    {
        Vector3i v1(v);
        v1 /= 2;
        EXPECT_DOUBLE_EQ(v1.x(), 0);
        EXPECT_DOUBLE_EQ(v1.y(), 1);
        EXPECT_DOUBLE_EQ(v1.z(), 1);
    }
    
    //operator== (const Vector3i&)
    {
        double e = v == Vector3i(1, 2, 3);
        EXPECT_TRUE(e);
    }
    
    //operator!= (const Vector3i&)
    {
        double e = v == Vector3i(1, 3, 3);
        EXPECT_FALSE(e);
    }

    //bool operator > (const Vector2 &vect) const;
    //bool operator >= (const Vector2 &vect) const;
    //bool operator < (const Vector2 &vect) const;
    //bool operator <= (const Vector2 &vect) const;
    {
        Vector3i ref(4, 5, 6);

        Vector3i v1(5, 5, 6);
        Vector3i v2(4, 6, 6);
        Vector3i v3(4, 5, 7);
        Vector3i v4(5, 6, 7);

        Vector3i v5(3, 5, 6);
        Vector3i v6(4, 4, 6);
        Vector3i v7(4, 5, 5);
        Vector3i v8(3, 4, 5);

        //operator>
        EXPECT_FALSE(ref > ref);
        EXPECT_FALSE(v1 > ref);
        EXPECT_FALSE(v2 > ref);
        EXPECT_FALSE(v3 > ref);
        EXPECT_TRUE(v4 > ref);

        EXPECT_FALSE(v5 > ref);
        EXPECT_FALSE(v6 > ref);
        EXPECT_FALSE(v7 > ref);
        EXPECT_FALSE(v8 > ref);

        //operator>=
        EXPECT_TRUE(ref >= ref);
        EXPECT_TRUE(v1 >= ref);
        EXPECT_TRUE(v2 >= ref);
        EXPECT_TRUE(v3 >= ref);
        EXPECT_TRUE(v4 >= ref);

        EXPECT_FALSE(v5 >= ref);
        EXPECT_FALSE(v6 >= ref);
        EXPECT_FALSE(v7 >= ref);
        EXPECT_FALSE(v8 >= ref);

        //operator<
        EXPECT_FALSE(ref < ref);
        EXPECT_FALSE(v1 < ref);
        EXPECT_FALSE(v2 < ref);
        EXPECT_FALSE(v3 < ref);
        EXPECT_FALSE(v4 < ref);

        EXPECT_FALSE(v5 < ref);
        EXPECT_FALSE(v6 < ref);
        EXPECT_FALSE(v7 < ref);
        EXPECT_TRUE(v8 < ref);

        //operator<=
        EXPECT_TRUE(ref <= ref);
        EXPECT_FALSE(v1 <= ref);
        EXPECT_FALSE(v2 <= ref);
        EXPECT_FALSE(v3 <= ref);
        EXPECT_FALSE(v4 <= ref);

        EXPECT_TRUE(v5 <= ref);
        EXPECT_TRUE(v6 <= ref);
        EXPECT_TRUE(v7 <= ref);
        EXPECT_TRUE(v8 <= ref);
    }
}

TEST(Vector3i, Functions)
{
    {
        Vector3i v;
        v.setX(1);
        v.setY(2);
        v.setZ(3);
        EXPECT_DOUBLE_EQ(v.x(), 1);
        EXPECT_DOUBLE_EQ(v.y(), 2);
        EXPECT_DOUBLE_EQ(v.z(), 3);
    }
    
    {
        Vector3i v;
        v.set(1, 2, 3);
        EXPECT_DOUBLE_EQ(v.x(), 1);
        EXPECT_DOUBLE_EQ(v.y(), 2);
        EXPECT_DOUBLE_EQ(v.z(), 3);
    }
    
    {
        Vector3i v(1, 2, 3);
        EXPECT_DOUBLE_EQ(v.x(), 1);
        EXPECT_DOUBLE_EQ(v.y(), 2);
        EXPECT_DOUBLE_EQ(v.z(), 3);
    }
    
    {
        Vector3i v(11, 22, 33);
        
        std::string s = v.toString();
        EXPECT_STREQ(s.c_str(), "(11, 22, 33)");
    }
    
}