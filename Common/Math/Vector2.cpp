
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

//-----------------------------------------------------------------------------
// implicit constructor for Vector2i
//
Vector2::Vector2(const Vector2i &iVector2i)
{
    set(iVector2i.x(), iVector2i.y());
}


//----------------------------------------------------------------------------
const double* Vector2::dataPointer() const
{ return &mData[0]; }

//---------------------------------------------------------------------------
Vector2	Vector2::divideComponents(const Vector2& v) const
{
    return Vector2(this->x() / v.x(), this->y() / v.y());
}

//----------------------------------------------------------------------------
double Vector2::dot(const Vector2& iV) const
{
    return this->operator*(iV);
}

//---------------------------------------------------------------------------
bool Vector2::isEqual(const Vector2& iV, double iEpsilon) const
{
    return Math::isEqual(x(), iV.x(), iEpsilon) &&
        Math::isEqual(y(), iV.y(), iEpsilon);
}

//---------------------------------------------------------------------------
double Vector2::norm() const
{
    return std::sqrt(mData[0]*mData[0] + mData[1]*mData[1]);
}

//---------------------------------------------------------------------------
Vector2& Vector2::normalize()
{
    double n = norm();
    
    //avoid dividing by zéro...
    if( !Math::isEqual( n, 0.0) )
    { (*this) /= this->norm(); }
    return (*this);
}

//---------------------------------------------------------------------------
double Vector2::normSquared() const
{
    return mData[0]*mData[0] + mData[1]*mData[1];
}

Vector2	Vector2::multiplyComponents(const Vector2& v) const
{
	return Vector2(this->x()*v.x(), this->y()*v.y());
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
Vector2& Vector2::operator-= (const Vector2 &iVect)
{
    *this = *this - iVect;
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

//----------------------------------------------------------------------------
bool Vector2::operator> (const Vector2& iV) const
{
    return mData[0] > iV.mData[0] &&
        mData[1] > iV.mData[1];
}

//----------------------------------------------------------------------------
bool Vector2::operator>= (const Vector2& iV) const
{
    return mData[0] >= iV.mData[0] &&
        mData[1] >= iV.mData[1];
}

//----------------------------------------------------------------------------
bool Vector2::operator< (const Vector2& iV) const
{
    return mData[0] < iV.mData[0] &&
        mData[1] < iV.mData[1];
}

//----------------------------------------------------------------------------
bool Vector2::operator<= (const Vector2& iV) const
{
    return mData[0] <= iV.mData[0] &&
        mData[1] <= iV.mData[1];
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

//---------------------------------------------------------------------------
// operator for scalar x Vector2
//
Vector2 Realisim::Math::operator*(double iV, const Vector2& iVect)
{
    return iVect * iV;
}
