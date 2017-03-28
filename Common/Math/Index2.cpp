
#include <cmath>
#include "Index.h"
#include <iomanip>
#include <sstream>

using namespace Realisim;
using namespace Math;

//-----------------------------------------------------------------------------
Index2::Index2() :
mX(0),
mY(0)
{}

//-----------------------------------------------------------------------------
Index2::Index2(int iV) :
mX(iV),
mY(iV)
{}

//-----------------------------------------------------------------------------
Index2::Index2(int iX, int iY) :
mX(iX),
mY(iY)
{}

//-----------------------------------------------------------------------------
const int* Index2::dataPointer() const
{ return &mX; }

//-----------------------------------------------------------------------------
double Index2::norm() const
{ return sqrt(mX*mX + mY*mY); }

//-----------------------------------------------------------------------------
Index2 Index2::operator+ (const Index2 &iIndex) const
{
    return Index2(x() + iIndex.x(),
        y() + iIndex.y());
}

//-----------------------------------------------------------------------------
Index2& Index2::operator+= (const Index2 &iIndex)
{
    mX += iIndex.x();
    mY += iIndex.y();
    return *this;
}

//-----------------------------------------------------------------------------
Index2 Index2::operator- (const Index2 &iIndex) const
{
    return Index2(x() - iIndex.x(),
        y() - iIndex.y());
}

//-----------------------------------------------------------------------------
Index2 Index2::operator- () const
{
    return Index2(-mX, -mY);
}

//-----------------------------------------------------------------------------
Index2& Index2::operator-= (const Index2 &iIndex)
{
    mX -= iIndex.x();
    mY -= iIndex.y();
    return *this;
}

//-----------------------------------------------------------------------------
Index2 Index2::operator* (double iV) const
{
    return Index2( (int)(iV*mX), (int)(iV*mY) );
}

//-----------------------------------------------------------------------------
Index2& Index2::operator*= (double iV)
{
    mX = (int)(iV*mX);
    mY = (int)(iV*mY);
    return *this;
}

//-----------------------------------------------------------------------------
Index2 Index2::operator/ (double iV) const
{
    return Index2((int)(mX / iV), (int)(mY / iV));
}

//-----------------------------------------------------------------------------
Index2& Index2::operator/= (double iV)
{
    mX = (int)(mX / iV);
    mY = (int)(mY / iV);
    return *this;
}

//-----------------------------------------------------------------------------
bool Index2::operator== ( const Index2& iV ) const
{
    return x() == iV.x() &&
        y() == iV.y();
}

//-----------------------------------------------------------------------------
bool Index2::operator!= ( const Index2& iV ) const
{
    return !( *this == iV );
}

//-----------------------------------------------------------------------------
void Index2::setX(int iV)
{ mX = iV; }

//-----------------------------------------------------------------------------
void Index2::setY(int iV)
{ mY = iV; }

//-----------------------------------------------------------------------------
void Index2::set(int iX, int iY)
{
    mX = iX;
    mY = iY;
}

//-----------------------------------------------------------------------------
std::string Index2::toString() const
{
    std::ostringstream oss;
    oss << std::fixed;
    oss << "(" << x() << ", " << y() << ")";
    return oss.str();
}

//-----------------------------------------------------------------------------
int Index2::x() const
{ return mX; }

//-----------------------------------------------------------------------------
int Index2::y() const
{ return mY; }

//---------------------------------------------------------------------------
// operator for scalar x Index2
//
Index2 Realisim::Math::operator*(double iV, const Index2& iIndex)
{
    return iIndex * iV;
}
