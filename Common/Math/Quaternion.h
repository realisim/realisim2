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
        Quaternion(const Quaternion &iQ) = default;
        Quaternion(double iX, double iY, double iZ, double iW);
        Quaternion(double iAngle, Vector3 iAxis);
        Quaternion& operator=(const Quaternion &iQ) = default;
        ~Quaternion();
        
        Quaternion conjugate() const;
        Quaternion& invert();
        Quaternion inverse();
        double norm() const;
        Quaternion& normalize();
        Quaternion operator+ (const Quaternion &iQ) const;
        Quaternion& operator+= (const Quaternion &iQ);
        Quaternion operator- (const Quaternion &iQ) const;
        Quaternion& operator-= (const Quaternion &iQ);
        Quaternion operator- () const;
        Quaternion operator* (double iValue) const;
        Quaternion& operator*= (double iValue);
        void set(double iX, double iY, double iZ, double iW);
        void setRotation(double iAngle, Vector3 iAxis);
        void setW(double iW);
        void setX(double iX);
        void setY(double iY);
        void setZ(double iZ);
        //Quaternion slerp(const Quaternion &iQ1, const Quaternion &iQ2, double iT);
        double w() const;
        double x() const;
        double y() const;
        double z() const;
        
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
