//!-----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include "Vector.h"
#include <string>

namespace Realisim
{
namespace Math
{
    class Quaternion
    {
    public:
        Quaternion();
        Quaternion(double iX, double iY, double iZ, double iW);
        Quaternion(const Quaternion &iQ) = default;
        Quaternion& operator=(const Quaternion &iQ) = default;
        ~Quaternion();
        
        Quaternion getConjugate() const;
        double getLength() const;
        Quaternion& invert();
        Quaternion inverse();
        Quaternion& normalize();
        Quaternion operator+ (const Quaternion &iQ) const;
        Quaternion& operator+= (const Quaternion &iQ);
        Quaternion operator- (const Quaternion &iQ) const;
        Quaternion& operator-= (const Quaternion &iQ);
        Quaternion operator- () const;
        Quaternion operator* (double iValue) const;
        Quaternion& operator*= (double iValue);
        Quaternion operator* (const Quaternion &iQ) const;
        Quaternion& operator*= (const Quaternion &iQ);
        void set(double iX, double iY, double iZ, double iW);
        void setRotation(double iAngle, const Vector3& iAxis);
        void setW(double iW);
        void setX(double iX);
        void setY(double iY);
        void setZ(double iZ);
        //Quaternion slerp(const Quaternion &iQ1, const Quaternion &iQ2, double iT);
        double w() const;
        double x() const;
        double y() const;
        double z() const;
        
        //Point3<T> multRotation( const Quaternion<T> &quat ) const;
        
        std::string toString() const;
        
    protected:
    private:
        
        double mX;
        double mY;
        double mZ;
        double mW;
    };
    
} //Math
} // fin du namespace realisim
