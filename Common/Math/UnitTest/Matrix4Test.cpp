
#define _USE_MATH_DEFINES // for C++ mostly M_PI

#include "gtest/gtest.h"
#include "Math/isEqual.h"
#include "Math/Matrix.h"
#include <cmath>

using namespace Realisim;
using namespace Math;

// define a macro to test equality in order to ease readability
// and maintain the line reporting mechanism of gtest
//
#define EXPECT_EQ_MATRIX4(m, r) \
{ \
    for(int i = 0; i < 4; ++i) \
        for(int j = 0; j < 4; ++j) \
        { \
            EXPECT_EQ(m(i,j), r[i*4 +j]) << "i: " << i << " j:" << j; \
        } \
}

#define EXPECT_IS_EQUAL_MATRIX4(m, r, e) \
{ \
    for(int i = 0; i < 4; ++i) \
        for(int j = 0; j < 4; ++j) \
        { \
            EXPECT_TRUE( isEqual( m(i,j), r[i*4 +j], e) ) << "i: " << i << " j:" << j; \
        } \
}


TEST(Matrix4, Constructor)
{
    double initializer[16] = {
        1,2,3,4,
        5,6,7,8,
        9,10,11,12,
        13,14,15,16};
    
    // Matrix4();
    {
        Matrix4 m;
        double result[16] ={
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1};
        
        EXPECT_EQ_MATRIX4(m, result);
    }
    
    // Matrix4( const Matrix4& ) = default;
    {
        Matrix4 m(initializer);
        Matrix4 m2(m);
        EXPECT_EQ_MATRIX4(m2, initializer);
    }
    
    //Matrix4( const double*, bool iRowMajor = true );
    {
        Matrix4 m(initializer);
        EXPECT_EQ_MATRIX4(m, initializer);
        
        //row major = false
        Matrix4 m2(initializer, false);
        double result[16] = {
            1,5,9,13,
            2,6,10,14,
            3,7,11,15,
            4,8,12,16};
        EXPECT_EQ_MATRIX4(m2, result);
        
        // 2 dimensional array
        double twoDimArray[4][4] = {
            {1,2,3,4},
            {5,6,7,8},
            {9,10,11,12},
            {13,14,15,16}
        };
        Matrix4 m3(twoDimArray[0]);
        EXPECT_EQ_MATRIX4(m3, initializer);
    }
    
    //Matrix4( Vector3 ); //translation
    {
        Vector3 t(1, 2, 3);
        Matrix4 m(t);
        double result[16] = {
            1,0,0,1,
            0,1,0,2,
            0,0,1,3,
            0,0,0,1};
        EXPECT_EQ_MATRIX4(m, result);
    }
    
    //Matrix4( Quaternion ); //rotation
    {
        // verified on wolfram with:
        // rotation 1+0i+5j+2k
        Quaternion q(0,5,2,1);
        q.normalize();
        Matrix4 m(q);
        double result[16] = {
            -14/15.0, -2/15.0, 1/3.0, 0,
            2/15.0, 11/15.0, 2/3.0, 0,
            -1/3.0, 2/3.0, -2/3.0, 0,
            0, 0, 0, 1};
        EXPECT_IS_EQUAL_MATRIX4(m, result, 1e-7)
    }
    
    //Matrix4( double, Vector3 ); //rotation (angle et axe)
    {
        // verified on wolfram alpha with:
        // rotate 30 degrees
        //
        double a = M_PI / 6.0; //30 deg counterclockwise
        Vector3 z(0, 0, 1);
        Matrix4 m(a, z);
        
        double result[16] = {
            std::sqrt(3.0)/2.0, -0.5, 0, 0,
            0.5, std::sqrt(3.0)/2.0 , 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
        EXPECT_IS_EQUAL_MATRIX4(m, result, 1e-7);
    }
    
    //Matrix4( Vector3, Vector3, Vector3 );
    {
        Vector3 x, y, z;
        x.set(1,2,3);
        y.set(4,5,6);
        z.set(7,8,9);
        Matrix4 m(x,y,z);
        double result[16] = {
            1,4,7,0,
            2,5,8,0,
            3,6,9,0,
            0,0,0,1};
        EXPECT_EQ_MATRIX4(m, result);
    }
}

TEST(Matrix4, operator)
{
    double initializer[16] = {
        1,2,3,4,
        5,6,7,8,
        9,10,11,12,
        13,14,15,16};
    
    //double operator()(int, int) const;
    {
        Matrix4 m(initializer);
        EXPECT_EQ(m(0,0), 1);
        EXPECT_EQ(m(0,1), 2);
        EXPECT_EQ(m(0,2), 3);
        EXPECT_EQ(m(0,3), 4);
        EXPECT_EQ(m(1,0), 5);
        EXPECT_EQ(m(1,1), 6);
        EXPECT_EQ(m(1,2), 7);
        EXPECT_EQ(m(1,3), 8);
        EXPECT_EQ(m(2,0), 9);
        EXPECT_EQ(m(2,1), 10);
        EXPECT_EQ(m(2,2), 11);
        EXPECT_EQ(m(2,3), 12);
        EXPECT_EQ(m(3,0), 13);
        EXPECT_EQ(m(3,1), 14);
        EXPECT_EQ(m(3,2), 15);
        EXPECT_EQ(m(3,3), 16);
    }

    //double& operator()(int, int);
    {
        //same as above
    }

    //bool operator== (const Matrix4&) const;
    {
        Matrix4 m(initializer);
        EXPECT_TRUE( m == m );
    }

    //bool operator!= (const Matrix4&) const;
    {
        Matrix4 m;
        Matrix4 m2(initializer);
        EXPECT_TRUE( m != m2 );
    }

    //Matrix4 operator* (const Matrix4&) const;
    {
        // verified on wolfram alpha
        Matrix4 m(initializer);
        Matrix4 m2 = m * m;
        double result[16] = {
            90, 100, 110, 120,
            202, 228, 254, 280,
            314, 356, 398, 440,
            426, 484, 542, 600};
        EXPECT_EQ_MATRIX4(m2, result);
    }

    //Matrix4& operator*= (const Matrix4&);
    {
        Matrix4 m(initializer);
        m *= m;
        double result[16] = {
            90, 100, 110, 120,
            202, 228, 254, 280,
            314, 356, 398, 440,
            426, 484, 542, 600};
        EXPECT_EQ_MATRIX4(m, result);
    }

    //Vector4 operator* (const Vector4&) const; 
    {
        // verified on wolfram alpha
        Matrix4 m(initializer);
        Vector4 v(1,2,3,4);
        Vector4 r = m * v;
        EXPECT_TRUE( r == Vector4(30, 70, 110, 150) );
    }
}

TEST(Matrix4, Functions)
{
    double initializer[16] = {
        1,2,3,4,
        5,6,7,8,
        9,10,11,12,
        13,14,15,16};
    
    //const double* getDataPointer() const;
    {
        Matrix4 m(initializer);
        EXPECT_EQ( 1, *(m.dataPointer()) );
    }

    //Quaternion getRotationAsQuaternion() const;
    {
        //tested in constructor
    }

    //Vector4 getRow(int) const;
    {
        Matrix4 m(initializer);
        Vector4 r = m.row(2);
        EXPECT_TRUE( r == Vector4(9, 10, 11, 12) );
    }

    //Vector3 getTranslationAsVector() const;
    {
        Matrix4 m(initializer);
        Vector3 r = m.translationAsVector();
        EXPECT_TRUE( r == Vector3(4, 8, 12) );
    }

    //Matrix4 inverse() const;
    {
        // this is a singular matrix... thus impossible to inverse
        // in that case, we return an identity matrix...
        //
        Matrix4 m(initializer);
        Matrix4 m2 = m.inverse();
        EXPECT_TRUE(m2 == Matrix4());
        
        // verified on wolfram alpha with:
        // inverse {{1,2,3,4},{5,1,7,8},{9,10,1,12},{13,14,15,1}}
        //
        double inversible[16] ={
            1,2,3,4,
            5,1,7,8,
            9,10,1,12,
            13,14,15,1};
        Matrix4 m3(inversible);
        Matrix4 m4 = m3.inverse();
        double result[16] = {
            -403/870.0, 22/145.0, 3/58.0, 8/435.0,
            37/145.0, -5/29.0, 4/145.0, 4/145.0,
            9/58.0, 4/145.0, -21/290.0, 4/145.0,
            53/435.0, 4/145.0, 4/145.0, -17/435.0};
        EXPECT_IS_EQUAL_MATRIX4(m4, result, 1e-7);
    }

    //Matrix4& invert();
    {
        // verified on wolfram alpha with:
        // inverse {{1,2,3,4},{5,1,7,8},{9,10,1,12},{13,14,15,1}}
        //
        double inversible[16] ={
            1,2,3,4,
            5,1,7,8,
            9,10,1,12,
            13,14,15,1};
        Matrix4 m(inversible);
        m.invert();
        double result[16] = {
            -403/870.0, 22/145.0, 3/58.0, 8/435.0,
            37/145.0, -5/29.0, 4/145.0, 4/145.0,
            9/58.0, 4/145.0, -21/290.0, 4/145.0,
            53/435.0, 4/145.0, 4/145.0, -17/435.0};
        EXPECT_IS_EQUAL_MATRIX4(m, result, 1e-7);
    }

    //bool isEqual( const Matrix4&, double = std::numeric_limits<double>::epsilon() ) const;
    {
        Matrix4 m(initializer);
        Matrix4 m2(initializer);
        EXPECT_TRUE(m.isEqual(m2));
    }

    //void setRotation( const Quaterniond& );
    {

    }

    //void setScale( const Vector3d& );
    {

    }

    //void setTranslation( const Vector3& );
    {
        Vector3 t(1, 2, 3);
        Matrix4 m;
        m.setTranslation(t);
        EXPECT_TRUE( t == m.translationAsVector() );
    }

    //Matrix4 transpose() const;
    {
        Matrix4 m(initializer);
        Matrix4 m2 = m.transpose();
        double result[16] = {
            1,5,9,13,
            2,6,10,14,
            3,7,11,15,
            4,8,12,16};
        EXPECT_EQ_MATRIX4(m2, result);
    }

    //std::string toString(int iPrecision = 3) const;
    {
        double initializer2[16] = {
            1.1,2.2,3.3,4.4,
            5.5,6.6,7.7,8.8,
            9.9,10.10,11.11,12.12,
            13.13,14.14,15.15,16.16};
        
        Matrix4 m(initializer2);
        std::string s1 = m.toString(1);
        std::string s2 = m.toString(2);
        
        std::string rs1 = "1.1,2.2,3.3,4.4\n5.5,6.6,7.7,8.8\n9.9,10.1,11.1,12.1\n13.1,14.1,15.2,16.2\n";
        std::string rs2 = "1.10,2.20,3.30,4.40\n5.50,6.60,7.70,8.80\n9.90,10.10,11.11,12.12\n13.13,14.14,15.15,16.16\n";
        
        EXPECT_STREQ(s1.c_str(), rs1.c_str());
        EXPECT_STREQ(s2.c_str(), rs2.c_str());
    }

}
