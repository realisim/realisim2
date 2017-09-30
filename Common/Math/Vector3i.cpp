
#include <cmath>
#include "VectorI.h"
#include <iomanip>
#include <sstream>

using namespace Realisim;
using namespace Math;

//------------------------------------------------------------------------
Vector3i::Vector3i() : mX(0), mY(0), mZ(0)
{}

//------------------------------------------------------------------------
Vector3i::Vector3i(int iV) : mX(iV), mY(iV), mZ(iV)
{}

//------------------------------------------------------------------------
Vector3i::Vector3i(int iX, int iY, int iZ) : mX(iX), mY(iY), mZ(iZ)
{}

//------------------------------------------------------------------------
Vector3i::Vector3i(const Vector2i& iIndex, int iZ) :
    mX(iIndex.x()),
    mY(iIndex.y()),
    mZ(iZ)
{}

//------------------------------------------------------------------------
const int* Vector3i::dataPointer() const
{ return &mX; }

//------------------------------------------------------------------------
double Vector3i::norm() const
{ return sqrt(mX*mX + mY*mY + mZ*mZ); }

//------------------------------------------------------------------------
double Vector3i::normSquare() const
{ return mX*mX + mY*mY + mZ*mZ; }

//------------------------------------------------------------------------
Vector3i Vector3i::operator+ (const Vector3i &iIndex) const
{
    return Vector3i(x() + iIndex.x(),
        y() + iIndex.y(),
        z() + iIndex.z());
}

//------------------------------------------------------------------------
Vector3i& Vector3i::operator+= (const Vector3i &iIndex)
{
    mX += iIndex.x();
    mY += iIndex.y();
    mZ += iIndex.z();
    return *this;
}

//------------------------------------------------------------------------
Vector3i Vector3i::operator- (const Vector3i &iIndex) const
{
    return Vector3i(x() - iIndex.x(),
        y() - iIndex.y(),
        z() - iIndex.z());
}

//------------------------------------------------------------------------
Vector3i Vector3i::operator- () const
{
    return Vector3i(-mX, -mY, -mZ);
}

//------------------------------------------------------------------------
Vector3i& Vector3i::operator-= (const Vector3i &iIndex)
{
    mX -= iIndex.x();
    mY -= iIndex.y();
    mZ -= iIndex.z();
    return *this;
}

//------------------------------------------------------------------------
Vector3i Vector3i::operator* (int iV) const
{
    return Vector3i(iV*mX, iV*mY, iV*mZ);
}

//------------------------------------------------------------------------
Vector3i& Vector3i::operator*= (int iV)
{
    mX = iV*mX;
    mY = iV*mY;
    mZ = iV*mZ;
    return *this;
}

//------------------------------------------------------------------------
Vector3i Vector3i::operator/ (int iV) const
{
    return Vector3i(mX/iV, mY / iV, mZ / iV);
}

//------------------------------------------------------------------------
Vector3i& Vector3i::operator/= (int iV)
{
    mX = mX / iV;
    mY = mY / iV;
    mZ = mZ / iV;
    return *this;
}

//------------------------------------------------------------------------
bool Vector3i::operator== ( const Vector3i& iV ) const
{
    return x() == iV.x() &&
        y() == iV.y() &&
        z() == iV.z();
}

//------------------------------------------------------------------------
bool Vector3i::operator!= ( const Vector3i& iV ) const
{
    return !( *this == iV );
}

//----------------------------------------------------------------------------
bool Vector3i::operator> (const Vector3i& iV) const
{
    return mX > iV.mX &&
        mY > iV.mY &&
        mZ > iV.mZ;
}

//----------------------------------------------------------------------------
bool Vector3i::operator>= (const Vector3i& iV) const
{
    return mX >= iV.mX &&
        mY >= iV.mY &&
        mZ >= iV.mZ;
}

//----------------------------------------------------------------------------
bool Vector3i::operator< (const Vector3i& iV) const
{
    return mX < iV.mX &&
        mY < iV.mY &&
        mZ < iV.mZ;
}

//----------------------------------------------------------------------------
bool Vector3i::operator<= (const Vector3i& iV) const
{
    return mX <= iV.mX &&
        mY <= iV.mY &&
        mZ <= iV.mZ;
}

//------------------------------------------------------------------------
void Vector3i::setX(int iX)
{ mX = iX; }

//------------------------------------------------------------------------
void Vector3i::setY(int iY)
{ mY = iY; }

//------------------------------------------------------------------------
void Vector3i::setZ(int iZ)
{ mZ = iZ; }

//------------------------------------------------------------------------
void Vector3i::set(int iX, int iY, int iZ)
{
    mX = iX;
    mY = iY;
    mZ = iZ;
}

//------------------------------------------------------------------------
std::string Vector3i::toString() const
{
    std::ostringstream oss;
    oss << std::fixed;
    oss << "(" << x() << ", " << y() << ", " << z() << ")";
    return oss.str();
}

//------------------------------------------------------------------------
int Vector3i::x() const
{ return mX; }

//------------------------------------------------------------------------
int Vector3i::y() const
{ return mY; }

//------------------------------------------------------------------------
int Vector3i::z() const
{ return mZ; }

//------------------------------------------------------------------------
Vector2i Vector3i::xy() const
{ return Vector2i(x(), y()); }

//------------------------------------------------------------------------
Vector2i Vector3i::xz() const
{ return Vector2i(x(), z()); }

//------------------------------------------------------------------------
Vector2i Vector3i::yz() const
{ return Vector2i(y(), z()); }

//---------------------------------------------------------------------------
// operator for scalar x Vector2i
//
Vector3i Realisim::Math::operator*(int iV, const Vector3i& iIndex)
{
    return iIndex * iV;
}

