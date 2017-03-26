
#pragma once
#include <string>

namespace Realisim
{
namespace Math
{
    //-------------------------------------------------------------------------
    class Index2
    {
    public:
        Index2();
        Index2(int iV);
        Index2(int iX, int iY);
        Index2(const Index2 &iIndex) = default;
        Index2& operator=(const Index2 &iIndex) = default;
        ~Index2() = default;

        const int* dataPointer() const;
        double norm() const;
        
        Index2 operator+ (const Index2 &iIndex) const;
        Index2& operator+= (const Index2 &iIndex);
        Index2 operator- (const Index2 &iIndex) const;
        Index2 operator- () const;
        Index2& operator-= (const Index2 &iIndex);
        Index2 operator* (double iV) const;
        Index2& operator*= (double iV);
        Index2 operator/ (double iV) const;
        Index2& operator/= (double iV);  
        bool operator== ( const Index2& ) const;
        bool operator!= ( const Index2& ) const;

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
    class Index3
    {
    public:
        Index3();
        Index3(int iV);
        Index3(int iX, int iY, int iZ);
        explicit Index3(const Index2& iIndex, int iZ);
        Index3(const Index3 &iiIndex) = default;
        Index3& operator=(const Index3 &iIndex) = default;
        ~Index3() = default;

        const int* dataPointer() const;
        double norm() const;
        
        Index3 operator+ (const Index3 &iIndex) const;
        Index3& operator+= (const Index3 &iIndex);
        Index3 operator- (const Index3 &iIndex) const;
        Index3 operator- () const;
        Index3& operator-= (const Index3 &iIndex);
        Index3 operator* (double iV) const;
        Index3& operator*= (double iV);
        Index3 operator/ (double iV) const;
        Index3& operator/= (double iV);
        bool operator== ( const Index3& iV ) const;
        bool operator!= ( const Index3& iV ) const;

        void setX(int iX);
        void setY(int iY);
        void setZ(int iZ);
        void set(int iX, int iY, int iZ);
        std::string toString() const;

        int x() const;
        int y() const;
        int z() const;
        Index2 xy() const;
        Index2 xz() const;
        Index2 yz() const;

    private:
        int mX;
        int mY;
        int mZ;
    };
    
    // Operator to multiply a scalar by a vector.
    // To represent: 2.0*v;
    //
    Index2 operator*(double iV, const Index2& iIndex);
    Index3 operator*(double iV, const Index3& iIndex);

}
}