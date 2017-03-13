#define _USE_MATH_DEFINES // for C++ mostly M_PI

#include "gtest/gtest.h"
#include "Math/isEqual.h"
#include "Math/Matrix.h"
#include <cmath>

using namespace Realisim;
using namespace Math;
using namespace std;

#define EXPECT_EQ_QUAT(a, b) \
{ \
    EXPECT_EQ(a.x(), b.x()); \
    EXPECT_EQ(a.y(), b.y()); \
    EXPECT_EQ(a.z(), b.z()); \
    EXPECT_EQ(a.w(), b.w()); \
}

#define EXPECT_IS_EQUAL_QUAT(a, b, e) \
{ \
    EXPECT_TRUE( isEqual( a.x(), b.x(), e ) ); \
    EXPECT_TRUE( isEqual( a.y(), b.y(), e ) ); \
    EXPECT_TRUE( isEqual( a.z(), b.z(), e ) ); \
    EXPECT_TRUE( isEqual( a.w(), b.w(), e ) ); \
}

TEST(Quaternion, constructor)
{
    //Quaternion();
    {
        Quaternion q;
        EXPECT_EQ(q.x(), 0.0);
        EXPECT_EQ(q.y(), 0.0);
        EXPECT_EQ(q.z(), 0.0);
        EXPECT_EQ(q.w(), 1.0);
    }

    //Quaternion(const Quaternion &iQ) = default;
    {
        Quaternion q(1,2,3,4);
        Quaternion q2(q);
        EXPECT_EQ_QUAT(q,q2);
    }

    //Quaternion(double iX, double iY, double iZ, double iW);
    {
        Quaternion q(1,2,3,4);
        EXPECT_EQ(q.x(), 1.0);
        EXPECT_EQ(q.y(), 2.0);
        EXPECT_EQ(q.z(), 3.0);
        EXPECT_EQ(q.w(), 4.0);
    }

    //Quaternion(double iAngle, const Vector3& iAxis);
    {
        // verified on wolfram alpha withL
        // rotation 1+0i+5j+2k
        //
        Quaternion q( 2.0 * acos( 1.0/sqrt(30.0) ), Vector3(0, sqrt(5/6.0), sqrt(2/15.0) ) );
        Quaternion r(0, 5, 2, 1);
        r.normalize();
        EXPECT_IS_EQUAL_QUAT(q, r, 1e-7);
    }

    //Quaternion& operator=(const Quaternion &iQ) = default;
    {
        Quaternion q(1,2,3,4);
        Quaternion q2;
        q2 = q;
        EXPECT_EQ_QUAT(q, q2);
    }
}


TEST(Quaternion, operator)
{
    Quaternion a(1,2,3,4);
    Quaternion b(5,6,7,8);
    //Quaternion operator+ (const Quaternion &iQ) const;
    {
        Quaternion q = a+b;
        EXPECT_EQ_QUAT(q, Quaternion(6,8,10,12));
    }

    //Quaternion& operator+= (const Quaternion &iQ);
    {
        Quaternion q = a;
        q += b;
        EXPECT_EQ_QUAT(q, Quaternion(6,8,10,12));
    }

    //Quaternion operator- (const Quaternion &iQ) const;
    {
        Quaternion q = a-b;
        EXPECT_EQ_QUAT(q, Quaternion(-4,-4,-4,-4));
    }

    //Quaternion& operator-= (const Quaternion &iQ);
    {
        Quaternion q = a;
        q -= b;
        EXPECT_EQ_QUAT(q, Quaternion(-4,-4,-4,-4));
    }

    //Quaternion operator- () const;
    {
        EXPECT_EQ_QUAT(-a, Quaternion(-1,-2,-3,-4));
    }

    //Quaternion operator* (double iValue) const;
    {
        Quaternion q = a*2;
        EXPECT_EQ_QUAT(q, Quaternion(2,4,6,8));
    }

    //Quaternion& operator*= (double iValue);
    {
        Quaternion q = a;
        q *= 2.0;
        EXPECT_EQ_QUAT(q, Quaternion(2,4,6,8));
    }
}

TEST(Quaternion, Functions)
{
    //Quaternion getConjugate() const;
    {
        Quaternion q(1,2,3,4);
        Quaternion r(-1,-2,-3,4);
        Quaternion c = q.conjugate();
        EXPECT_EQ_QUAT(c, r);
    }
    
    //double norm() const;
    {
        Quaternion q(1,2,3,4);
        double n = q.norm();
        EXPECT_DOUBLE_EQ(n, sqrt(30));
    }
    
    //Quaternion& invert();
    {
        Quaternion q(1,2,3,4);
        q.invert();
        Quaternion r(-1/30.0,-1/15.0,-1/10.0,2/15.0);
        EXPECT_EQ_QUAT(q, r);
    }
    
    //Quaternion inverse();
    {
        Quaternion q(1,2,3,4);
        Quaternion i = q.inverse();
        Quaternion r(-1/30.0,-1/15.0,-1/10.0,2/15.0);
        EXPECT_EQ_QUAT(i, r);
    }
    
    //Quaternion& normalize();
    {
        Quaternion q(1,2,3,4);
        q.normalize();
        Quaternion r(1/sqrt(30.0), sqrt(2/15.0), sqrt(3/10.0), 2*sqrt(2/15.0));
        EXPECT_EQ_QUAT(q, r);
    }
    
    //void set(double iX, double iY, double iZ, double iW);
    {
        Quaternion q;
        q.set(1,2,3,4);
        Quaternion r(1,2,3,4);
        EXPECT_EQ_QUAT(q, r);
    }
    
    //void setRotation(double iAngle, Vector3 iAxis);
    {
        Quaternion q;
        q.setRotation( 2.0 * acos( 1.0/sqrt(30.0) ),
                      Vector3(0, sqrt(5/6.0), sqrt(2/15.0) ) );
        Quaternion r(0, 5, 2, 1);
        r.normalize();
        EXPECT_IS_EQUAL_QUAT(q, r, 1e-7);
    }
    
    //void setW(double iW);
    {
        Quaternion q;
        q.setW(4);
        EXPECT_DOUBLE_EQ(4, q.w());
    }
    
    //void setX(double iX);
    {
        Quaternion q;
        q.setX(1);
        EXPECT_DOUBLE_EQ(1, q.x());
    }
    
    //void setY(double iY);
    {
        Quaternion q;
        q.setY(2);
        EXPECT_DOUBLE_EQ(2, q.y());
    }
    
    //void setZ(double iZ);
    {
        Quaternion q;
        q.setZ(3);
        EXPECT_DOUBLE_EQ(3, q.z());
    }
    
}

