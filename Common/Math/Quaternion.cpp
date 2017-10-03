
#include <cmath>
#include "IsEqual.h"
#include "Quaternion.h"
#include <string>

using namespace Realisim;
using namespace Math;

Quaternion::Quaternion() : mX(0.0), mY(0.0), mZ(0.0), mW(1.0)
{
}

//-----------------------------------------------------------------------------
Quaternion::Quaternion(double iX, double iY, double iZ, double iW) :
mX(iX), mY(iY), mZ(iZ), mW(iW)
{}

//-----------------------------------------------------------------------------
Quaternion::Quaternion(double iAngle, Vector3 iAxis)
{ setRotation(iAngle, iAxis); }

//----------------------------------------------------------------------------
Quaternion::~Quaternion()
{}

//----------------------------------------------------------------------------
void Quaternion::setW(double iW)
{
    mW = iW;
}

//----------------------------------------------------------------------------
void Quaternion::setX(double iX)
{
    mX = iX;
}

//----------------------------------------------------------------------------
void Quaternion::setY(double iY)
{
    mY = iY;
}

//----------------------------------------------------------------------------
void Quaternion::setZ(double iZ)
{
    mZ = iZ;
}

//----------------------------------------------------------------------------
void Quaternion::set(double iX, double iY, double iZ, double iW)
{
    mW = iW;
    mX = iX;
    mY = iY;
    mZ = iZ;
}

//----------------------------------------------------------------------------
double Quaternion::w() const
{
    return mW;
}

//----------------------------------------------------------------------------
double Quaternion::x() const
{
    return mX;
}

//----------------------------------------------------------------------------
double Quaternion::y() const
{
    return mY;
}

//----------------------------------------------------------------------------
double Quaternion::z() const
{
    return mZ;
}

//----------------------------------------------------------------------------
std::string Quaternion::toString() const
{
    std::string r;
    r = "(" + std::to_string(mX) + ", " +
    std::to_string(mY) + ", " +
    std::to_string(mZ) + ", " +
    std::to_string(mW) + ")";
    return r;
}

//----------------------------------------------------------------------------
double Quaternion::norm() const
{
    return std::sqrt(mW*mW + mX*mX + mY*mY + mZ*mZ);
}

//----------------------------------------------------------------------------
Quaternion& Quaternion::normalize()
{
    const double magnitude = norm();
    //si length est 0, on ne fait pas la division!
    if ( !isEqual(magnitude, 0.0, 1e-7) )
    {
        mW = mW / magnitude;
        mX = mX / magnitude;
        mY = mY / magnitude;
        mZ = mZ / magnitude;
    }
    return *this;
}

//----------------------------------------------------------------------------
//addition de quaternion
Quaternion Quaternion::operator+  (const Quaternion &quat) const
{
    
    Quaternion result(*this);
    result += quat;
    return result;
}

//----------------------------------------------------------------------------
//addition de quaternion
Quaternion& Quaternion::operator+=  (const Quaternion &quat)
{
    mW += quat.mW;
    mX += quat.mX;
    mY += quat.mY;
    mZ += quat.mZ;
    return *this;
}

//----------------------------------------------------------------------------
//soustraction de quaternion
Quaternion Quaternion::operator-  (const Quaternion &quat) const
{
    
    Quaternion result(*this);
    result -= quat;
    return result;
}

//----------------------------------------------------------------------------
//soustraction de quaternion
Quaternion& Quaternion::operator-=  (const Quaternion &quat)
{
    mW -= quat.mW;
    mX -= quat.mX;
    mY -= quat.mY;
    mZ -= quat.mZ;
    return *this;
}

//----------------------------------------------------------------------------
//inversion du quaternion
Quaternion Quaternion::operator-  () const
{
    
    Quaternion result(*this);
    result *= -1;
    return result;
}

//----------------------------------------------------------------------------
//multiplication par un scalaire
Quaternion Quaternion::operator* (double iValue) const
{
    Quaternion result(*this);
    return result *= iValue;
}

//----------------------------------------------------------------------------
//multiplication par un scalaire
Quaternion& Quaternion::operator*= (double iValue)
{
    mW = mW * iValue;
    mX = mX * iValue;
    mY = mY * iValue;
    mZ = mZ * iValue;
    return *this;
}

//----------------------------------------------------------------------------
Quaternion Quaternion::getConjugate() const
{
    return Quaternion(-mX, -mY, -mZ, mW);
}

//----------------------------------------------------------------------------
Quaternion Quaternion::inverse()
{
    Quaternion r(*this);
    double squareNorm = 1.0 / (r.x()*r.x() + r.y()*r.y() + r.z()*r.z() + r.w()*r.w());
    r = r.getConjugate() * squareNorm;
    return r;
}

//----------------------------------------------------------------------------
Quaternion& Quaternion::invert()
{
    double squareNorm = 1.0 / (mX * mX + mY * mY + mZ* mZ + mW * mW);
    *this = (*this).getConjugate() * squareNorm;
    return *this;
}

//!---------------------------------------------------------------------------
//! \brief  Fonction permettant d'initialiser un quaternion pour une rotation.
//!
//! \param angle angle que l'on doit tourner autour de l'axe EN RADIAN!!!
//! \param axisX
//! \param axisY le vecteur a tourner autour...
//! \param axisZ
//!---------------------------------------------------------------------------
void Quaternion::setRotation(double iAngle, Vector3 iAxis)
{
    iAxis.normalize();
    
    double sinTmp = std::sin(iAngle/2.0);
    
    mX = iAxis.x() * sinTmp;
    mY = iAxis.y() * sinTmp;
    mZ = iAxis.z() * sinTmp;
    mW = std::cos(iAngle/2.0);
}
