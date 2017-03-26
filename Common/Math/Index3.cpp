
#include <cmath>
#include "Index.h"
#include <iomanip>
#include <sstream>

using namespace Realisim;
using namespace Math;

//------------------------------------------------------------------------
Index3::Index3() : mX(0), mY(0), mZ(0)
{}

//------------------------------------------------------------------------
Index3::Index3(int iV) : mX(iV), mY(iV), mZ(iV)
{}

//------------------------------------------------------------------------
Index3::Index3(int iX, int iY, int iZ) : mX(iX), mY(iY), mZ(iZ)
{}

//------------------------------------------------------------------------
Index3::Index3(const Index2& iIndex, int iZ) :
    mX(iIndex.x()),
    mY(iIndex.y()),
    mZ(iZ)
{}

//------------------------------------------------------------------------
const int* Index3::dataPointer() const
{ return &mX; }

//------------------------------------------------------------------------
double Index3::norm() const
{ return sqrt(mX*mX + mY*mY + mZ*mZ); }

//------------------------------------------------------------------------
Index3 Index3::operator+ (const Index3 &iIndex) const
{
    return Index3(x() + iIndex.x(),
        y() + iIndex.y(),
        z() + iIndex.z());
}

//------------------------------------------------------------------------
Index3& Index3::operator+= (const Index3 &iIndex)
{
    mX += iIndex.x();
    mY += iIndex.y();
    mZ += iIndex.z();
    return *this;
}

//------------------------------------------------------------------------
Index3 Index3::operator- (const Index3 &iIndex) const
{
    return Index3(x() - iIndex.x(),
        y() - iIndex.y(),
        z() - iIndex.z());
}

//------------------------------------------------------------------------
Index3 Index3::operator- () const
{
    return Index3(-mX, -mY, -mZ);
}

//------------------------------------------------------------------------
Index3& Index3::operator-= (const Index3 &iIndex)
{
    mX -= iIndex.x();
    mY -= iIndex.y();
    mZ -= iIndex.z();
    return *this;
}

//------------------------------------------------------------------------
Index3 Index3::operator* (double iV) const
{
    return Index3(iV*mX, iV*mY, iV*mZ);
}

//------------------------------------------------------------------------
Index3& Index3::operator*= (double iV)
{
    mX *= iV;
    mY *= iV;
    mZ *= iV;
    return *this;
}

//------------------------------------------------------------------------
Index3 Index3::operator/ (double iV) const
{
    return Index3(mX/iV, mY/iV, mZ/iV);
}

//------------------------------------------------------------------------
Index3& Index3::operator/= (double iV)
{
    mX /= iV;
    mY /= iV;
    mZ /= iV;
    return *this;
}

//------------------------------------------------------------------------
bool Index3::operator== ( const Index3& iV ) const
{
    return x() == iV.x() &&
        y() == iV.y() &&
        z() == iV.z();
}

//------------------------------------------------------------------------
bool Index3::operator!= ( const Index3& iV ) const
{
    return !( *this == iV );
}

//------------------------------------------------------------------------
void Index3::setX(int iX)
{ mX = iX; }

//------------------------------------------------------------------------
void Index3::setY(int iY)
{ mY = iY; }

//------------------------------------------------------------------------
void Index3::setZ(int iZ)
{ mZ = iZ; }

//------------------------------------------------------------------------
void Index3::set(int iX, int iY, int iZ)
{
    mX = iX;
    mY = iY;
    mZ = iZ;
}

//------------------------------------------------------------------------
std::string Index3::toString() const
{
    std::ostringstream oss;
    oss << std::fixed;
    oss << "(" << x() << ", " << y() << ", " << z() << ")";
    return oss.str();
}

//------------------------------------------------------------------------
int Index3::x() const
{ return mX; }

//------------------------------------------------------------------------
int Index3::y() const
{ return mY; }

//------------------------------------------------------------------------
int Index3::z() const
{ return mZ; }

//------------------------------------------------------------------------
Index2 Index3::xy() const
{ return Index2(x(), y()); }

//------------------------------------------------------------------------
Index2 Index3::xz() const
{ return Index2(x(), z()); }

//------------------------------------------------------------------------
Index2 Index3::yz() const
{ return Index2(y(), z()); }

//---------------------------------------------------------------------------
// operator for scalar x Index2
//
Index3 Realisim::Math::operator*(double iV, const Index3& iIndex)
{
    return iIndex * iV;
}

