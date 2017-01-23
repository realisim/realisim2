
#include <cmath>
#include <iomanip>
#include "Math/IsEqual.h"
#include "Math/Vector.h"
#include <sstream>

using namespace Realisim;
    using namespace Math;

Vector2::Vector2()
{ mData[0] = mData[1] = 0.0; }

//-----------------------------------------------------------------------------
Vector2::Vector2(double iV)
{ mData[0] = mData[1] = iV; }

//-----------------------------------------------------------------------------
Vector2::Vector2(double iX, double iY)
{ set(iX, iY); }

//----------------------------------------------------------------------------
const double* Vector2::getDataPointer() const
{ return &mData[0]; }

//---------------------------------------------------------------------------
bool Vector2::isEqual(const Vector2& iV) const
{
    return Math::isEqual(x(), iV.x()) &&
        Math::isEqual(y(), iV.y());
}

//---------------------------------------------------------------------------
double Vector2::norm() const
{
    return std::sqrt(mData[0]*mData[0] + mData[1]*mData[1]);
}

//---------------------------------------------------------------------------
Vector2& Vector2::normalize()
{
    (*this) /= this->norm();
    return (*this);
}

//----------------------------------------------------------------------------
// strictement egale, pour une comparaison un peu plus permissive, voir
// Math::isEqual( const Vector2&, const Vector2&, double )
bool Vector2::operator== (const Vector2& iV) const
{
    return x() == iV.x() &&
        y() == iV.y();
}

//----------------------------------------------------------------------------
bool Vector2::operator!= ( const Vector2& iV ) const
{ return !( *this == iV ); }

//----------------------------------------------------------------------------
Vector2 Vector2::operator+ (const Vector2 &vect) const
{
    Vector2 r;

    r.mData[0] = x() + vect.x();
    r.mData[1] = y() + vect.y();

    return r;
}

//----------------------------------------------------------------------------
Vector2& Vector2::operator+= (const Vector2 &vect)
{
    mData[0] += vect.x();
    mData[1] += vect.y();
    return *this;
}

//----------------------------------------------------------------------------
Vector2 Vector2::operator- (const Vector2 &vect) const
{
    Vector2 r;

    r.mData[0] = x() - vect.x();
    r.mData[1] = y() - vect.y();

    return r;
}

//----------------------------------------------------------------------------
Vector2 Vector2::operator- () const
{ 
    Vector2 r;
       
    r.mData[0] = -mData[0];
    r.mData[1] = -mData[1];

    return r;
}

//----------------------------------------------------------------------------
Vector2& Vector2::operator-= (const Vector2 &vect)
{
    mData[0] -= vect.x();
    mData[1] -= vect.y();
    return *this;
}

//----------------------------------------------------------------------------
Vector2 Vector2::operator* (double val) const
{
    Vector2 r;

    r.mData[0] = x() * val;
    r.mData[1] = y() * val;

    return r;
}

//----------------------------------------------------------------------------
Vector2& Vector2::operator*= (double val)
{
    mData[0] *= val;
    mData[1] *= val;
    return *this;
}

//----------------------------------------------------------------------------
Vector2 Vector2::operator/ (double val) const
{
    double vTmp;
    Vector2 vect(*this);

    const double epsilon = std::numeric_limits<double>::epsilon();
    if(val>epsilon || val<-epsilon)
      vTmp=((double)1.0)/(double)val;
    else
      vTmp=(double)0.0;
      
    vect *= vTmp;    
    return vect;
}

//----------------------------------------------------------------------------
Vector2& Vector2::operator/= (double val)
{
    double vTmp;

    const double epsilon = std::numeric_limits<double>::epsilon();
    if(val>epsilon || val<-epsilon)
      vTmp=((double)1.0)/(double)val;
    else
      vTmp=(double)0.0;
      
        *this *= vTmp;
    return *this;
}

//----------------------------------------------------------------------------
// scalar product
double Vector2::operator* (const Vector2 &vect) const
{
    return ( x() * vect.x() + y() * vect.y() );
}

//---------------------------------------------------------------------------
void Vector2::set(double iX, double iY)
{ mData[0] = iX; mData[1] = iY; }

//---------------------------------------------------------------------------
void Vector2::setX(double iX)
{ mData[0] = iX; }

//---------------------------------------------------------------------------
void Vector2::setY(double iY)
{ mData[1] = iY; }

//----------------------------------------------------------------------------
std::string Vector2::toString(int iPrecision /*=3*/) const
{ 
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(iPrecision);
  oss << "(" << x() << ", " << y() << ")";
  return oss.str();
}

//---------------------------------------------------------------------------
double Vector2::x() const
{ return mData[0]; }

//---------------------------------------------------------------------------
double Vector2::y() const
{ return mData[1]; }
