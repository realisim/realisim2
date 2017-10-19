
#pragma once

#include "VectorI.h"
#include <limits>
#include <string>

namespace Realisim
{
namespace Math
{
    //-------------------------------------------------------------------------
    class Vector2
    {
    public:
        Vector2();
        Vector2(double iV);
        Vector2(double iX, double iY);
        Vector2(const Vector2i &iVector2i);
        Vector2(const Vector2 &iVect) = default;
        Vector2& operator=(const Vector2 &vect) = default;
        ~Vector2() = default;

        const double* dataPointer() const;
        bool isEqual(const Vector2&, double iEpsilon = std::numeric_limits<double>::epsilon() ) const;
        double norm() const;
        Vector2& normalize();
        double normSquare() const;

		Vector2	multiplyComponents(const Vector2& v) const;

        Vector2 operator+ (const Vector2 &vect) const;
        Vector2& operator+= (const Vector2 &vect);
        Vector2 operator- (const Vector2 &vect) const;
        Vector2 operator- () const;
        Vector2& operator-= (const Vector2 &vect);
        Vector2 operator* (double) const;
        Vector2& operator*= (double);
        Vector2 operator/ (double) const;
        Vector2& operator/= (double);  
        bool operator== ( const Vector2& ) const;
        bool operator!= ( const Vector2& ) const;
        double operator* (const Vector2 &vect) const; // dot product
        bool operator > (const Vector2 &vect) const;
        bool operator >= (const Vector2 &vect) const;
        bool operator < (const Vector2 &vect) const;
        bool operator <= (const Vector2 &vect) const;

        void setX(double);
        void setY(double);
        void set(double iX, double iY);

        double x() const;
        double y() const; 
        std::string toString(int iPrecision =3) const;

    protected:
        double mData[2];
    };
    
    //-------------------------------------------------------------------------
    class Vector3 
    {
    public:
        Vector3();
        Vector3(double iV);
        Vector3(double iX, double iY, double iZ);
        explicit Vector3(const Vector2& iVect2, double iZ);
        Vector3(const Vector3 &iVect) = default;
        Vector3& operator=(const Vector3 &vect) = default;
        ~Vector3() = default;

        const double* dataPointer() const;
        bool isEqual(const Vector3&, double iEpsilon = std::numeric_limits<double>::epsilon() ) const;
        double norm() const;
        double normSquare() const;
        Vector3& normalize();

		Vector3	multiplyComponents(const Vector3&) const;
        
        Vector3 operator+ (const Vector3 &vect) const;
        Vector3& operator+= (const Vector3 &vect);
        Vector3 operator- (const Vector3 &vect) const;
        Vector3 operator- () const;
        Vector3& operator-= (const Vector3 &vect);
        Vector3 operator* (double) const;
        Vector3& operator*= (double);
        Vector3 operator/ (double) const;
        Vector3& operator/= (double);  
        bool operator== ( const Vector3& ) const;
        bool operator!= ( const Vector3& ) const;
        Vector3 operator^ (const Vector3 &vect) const; // scalar product
        double operator* (const Vector3 &vect) const; // dot product
        bool operator > (const Vector3 &vect) const;
        bool operator >= (const Vector3 &vect) const;
        bool operator < (const Vector3 &vect) const;
        bool operator <= (const Vector3 &vect) const;

        void setX(double);
        void setY(double);
        void setZ(double);
        void set(double iX, double iY, double iZ);

        double x() const;
        double y() const;
        double z() const;
        Vector2 xy() const;
        Vector2 xz() const;
        Vector2 yz() const;
        std::string toString(int iPrecision =3) const;

    protected:
        double mData[3];
    };
    
    //-------------------------------------------------------------------------
    class Vector4
    {
    public:
        Vector4();
        Vector4(double x, double y, double z, double w);
        explicit Vector4(const Vector2& v2, double z, double w);
        explicit Vector4(const Vector3& v3, double w);
        Vector4(double v);
        Vector4(const Vector4&) = default;
        Vector4& operator=(const Vector4&) = default;
        ~Vector4() = default;
        
        double norm() const;
        Vector4& normalize();
        double normSquare() const;

		Vector4	multiplyComponents(const Vector4& v) const;

        const double* dataPointer() const;
        bool isEqual(const Vector4&, double iEpsizion = std::numeric_limits<double>::epsilon() ) const;
        Vector4 operator+ (const Vector4&);
        Vector4& operator+= (const Vector4&);
        Vector4 operator- (const Vector4&);
        Vector4 operator- () const;
        Vector4& operator-= (const Vector4&);
        Vector4 operator* (double) const;
        Vector4& operator*= (double);
        Vector4 operator/ (double) const;
        Vector4& operator/= (double);
        bool operator== ( const Vector4& ) const;
        bool operator!= ( const Vector4& ) const;
//Vector4 operator^ (const Vector4 &vect) const; // scalar product
        double operator* (const Vector4 &vect) const; // dot product
        bool operator> (const Vector4 &vect) const;
        bool operator>= (const Vector4 &vect) const;
        bool operator< (const Vector4 &vect) const;
        bool operator<= (const Vector4 &vect) const;
        
        void set(double x, double y, double z, double w);
        void setX(double x);
        void setY(double y);
        void setZ(double z);
        void setW(double w);
        std::string toString(int iPrecision = 3) const;
        double x() const;
        double y() const;
        double z() const;
        double w() const;
        Vector3 xyz() const;

    protected:
        double mData[4];
    };
 
    // Operator to multiply a scalar by a vector.
    // To represent: 2.0*v;
    //
    Vector2 operator*(double iV, const Vector2& iVect);
    Vector3 operator*(double iV, const Vector3& iVect);
    Vector4 operator*(double iV, const Vector4& iVect);
}
}