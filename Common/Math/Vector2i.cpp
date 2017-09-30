
#include <cmath>
#include "VectorI.h"
#include <iomanip>
#include <sstream>

using namespace Realisim;
using namespace Math;

//-----------------------------------------------------------------------------
Vector2i::Vector2i() :
mX(0),
mY(0)
{}

//-----------------------------------------------------------------------------
Vector2i::Vector2i(int iV) :
mX(iV),
mY(iV)
{}

//-----------------------------------------------------------------------------
Vector2i::Vector2i(int iX, int iY) :
mX(iX),
mY(iY)
{}

//-----------------------------------------------------------------------------
const int* Vector2i::dataPointer() const
{ return &mX; }

//-----------------------------------------------------------------------------
double Vector2i::norm() const
{ return sqrt(mX*mX + mY*mY); }

//-----------------------------------------------------------------------------
double Vector2i::normSquare() const
{ return mX*mX + mY*mY; }

//-----------------------------------------------------------------------------
Vector2i Vector2i::operator+ (const Vector2i &iIndex) const
{
    return Vector2i(x() + iIndex.x(),
        y() + iIndex.y());
}

//-----------------------------------------------------------------------------
Vector2i& Vector2i::operator+= (const Vector2i &iIndex)
{
    mX += iIndex.x();
    mY += iIndex.y();
    return *this;
}

//-----------------------------------------------------------------------------
Vector2i Vector2i::operator- (const Vector2i &iIndex) const
{
    return Vector2i(x() - iIndex.x(),
        y() - iIndex.y());
}

//-----------------------------------------------------------------------------
Vector2i Vector2i::operator- () const
{
    return Vector2i(-mX, -mY);
}

//-----------------------------------------------------------------------------
Vector2i& Vector2i::operator-= (const Vector2i &iIndex)
{
    mX -= iIndex.x();
    mY -= iIndex.y();
    return *this;
}

//-----------------------------------------------------------------------------
Vector2i Vector2i::operator* (int iV) const
{
    return Vector2i( iV*mX, iV*mY );
}

//-----------------------------------------------------------------------------
Vector2i& Vector2i::operator*= (int iV)
{
    mX = iV*mX;
    mY = iV*mY;
    return *this;
}

//-----------------------------------------------------------------------------
Vector2i Vector2i::operator/ (int iV) const
{
    return Vector2i(mX / iV, mY / iV);
}

//-----------------------------------------------------------------------------
Vector2i& Vector2i::operator/= (int iV)
{
    mX = mX / iV;
    mY = mY / iV;
    return *this;
}

//-----------------------------------------------------------------------------
bool Vector2i::operator== ( const Vector2i& iV ) const
{
    return x() == iV.x() &&
        y() == iV.y();
}

//-----------------------------------------------------------------------------
bool Vector2i::operator!= ( const Vector2i& iV ) const
{
    return !( *this == iV );
}

//----------------------------------------------------------------------------
bool Vector2i::operator> (const Vector2i& iV) const
{
    return mX > iV.mX &&
        mY > iV.mY;
}

//----------------------------------------------------------------------------
bool Vector2i::operator>= (const Vector2i& iV) const
{
    return mX >= iV.mX &&
        mY >= iV.mY;
}

//----------------------------------------------------------------------------
bool Vector2i::operator< (const Vector2i& iV) const
{
    return mX < iV.mX &&
        mY < iV.mY;
}

//----------------------------------------------------------------------------
bool Vector2i::operator<= (const Vector2i& iV) const
{
    return mX <= iV.mX &&
        mY <= iV.mY;
}

//-----------------------------------------------------------------------------
void Vector2i::setX(int iV)
{ mX = iV; }

//-----------------------------------------------------------------------------
void Vector2i::setY(int iV)
{ mY = iV; }

//-----------------------------------------------------------------------------
void Vector2i::set(int iX, int iY)
{
    mX = iX;
    mY = iY;
}

//-----------------------------------------------------------------------------
std::string Vector2i::toString() const
{
    std::ostringstream oss;
    oss << std::fixed;
    oss << "(" << x() << ", " << y() << ")";
    return oss.str();
}

//-----------------------------------------------------------------------------
int Vector2i::x() const
{ return mX; }

//-----------------------------------------------------------------------------
int Vector2i::y() const
{ return mY; }

//---------------------------------------------------------------------------
// operator for scalar x Vector2i
//
Vector2i Realisim::Math::operator*(int iV, const Vector2i& iIndex)
{
    return iIndex * iV;
}
