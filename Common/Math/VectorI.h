
#pragma once
#include <string>

namespace Realisim
{
namespace Math
{
    //-------------------------------------------------------------------------
    class Vector2i
    {
    public:
        Vector2i();
        Vector2i(int iV);
        Vector2i(int iX, int iY);
        Vector2i(const Vector2i &iIndex) = default;
        Vector2i& operator=(const Vector2i &iIndex) = default;
        ~Vector2i() = default;

        const int* dataPointer() const;
        double norm() const;
		double normSquare() const;

        Vector2i operator+ (const Vector2i &iIndex) const;
        Vector2i& operator+= (const Vector2i &iIndex);
        Vector2i operator- (const Vector2i &iIndex) const;
        Vector2i operator- () const;
        Vector2i& operator-= (const Vector2i &iIndex);
        Vector2i operator* (int iV) const;
        Vector2i& operator*= (int iV);
        Vector2i operator/ (int iV) const;
        Vector2i& operator/= (int iV);
        bool operator== ( const Vector2i& ) const;
        bool operator!= ( const Vector2i& ) const;
        bool operator > (const Vector2i &vect) const;
        bool operator >= (const Vector2i &vect) const;
        bool operator < (const Vector2i &vect) const;
        bool operator <= (const Vector2i &vect) const;

        void setX(int);
        void setY(int);
        void set(int iX, int iY);
        std::string toString() const;

        int x() const;
        int y() const; 

    private:
        int mX;
        int mY;
    };

    //-------------------------------------------------------------------------
    class Vector3i
    {
    public:
        Vector3i();
        Vector3i(int iV);
        Vector3i(int iX, int iY, int iZ);
        explicit Vector3i(const Vector2i& iIndex, int iZ);
        Vector3i(const Vector3i &iiIndex) = default;
        Vector3i& operator=(const Vector3i &iIndex) = default;
        ~Vector3i() = default;

        const int* dataPointer() const;
        double norm() const;
		double normSquare() const;
        
        Vector3i operator+ (const Vector3i &iIndex) const;
        Vector3i& operator+= (const Vector3i &iIndex);
        Vector3i operator- (const Vector3i &iIndex) const;
        Vector3i operator- () const;
        Vector3i& operator-= (const Vector3i &iIndex);
        Vector3i operator* (int iV) const;
        Vector3i& operator*= (int iV);
        Vector3i operator/ (int iV) const;
        Vector3i& operator/= (int iV);
        bool operator== ( const Vector3i& iV ) const;
        bool operator!= ( const Vector3i& iV ) const;
        bool operator > (const Vector3i &vect) const;
        bool operator >= (const Vector3i &vect) const;
        bool operator < (const Vector3i &vect) const;
        bool operator <= (const Vector3i &vect) const;

        void setX(int iX);
        void setY(int iY);
        void setZ(int iZ);
        void set(int iX, int iY, int iZ);
        std::string toString() const;

        int x() const;
        int y() const;
        int z() const;
        Vector2i xy() const;
        Vector2i xz() const;
        Vector2i yz() const;

    private:
        int mX;
        int mY;
        int mZ;
    };
    
    // Operator to multiply a scalar by a vector.
    // To represent: 2.0*v;
    //
    Vector2i operator*(int iV, const Vector2i& iIndex);
    Vector3i operator*(int iV, const Vector3i& iIndex);

}
}