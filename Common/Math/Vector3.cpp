
#include <cmath>
#include <iomanip>
#include "Math/IsEqual.h"
#include "Math/Vector.h"
#include <sstream>

using namespace Realisim;
    using namespace Math;

Vector3::Vector3()
{ mData[0] = mData[1] = mData[2] = 0.0; }

//-----------------------------------------------------------------------------
Vector3::Vector3(double iV)
{ mData[0] = mData[1] = mData[2] = iV; }

//-----------------------------------------------------------------------------
Vector3::Vector3(double iX, double iY, double iZ)
{ set(iX, iY, iZ); }

//-----------------------------------------------------------------------------
Vector3::Vector3(const Vector2& iVect2, double iZ)
{ set(iVect2.x(), iVect2.y(), iZ); }

//----------------------------------------------------------------------------
const double* Vector3::dataPointer() const
{ return &mData[0]; }

//---------------------------------------------------------------------------
bool Vector3::isEqual(const Vector3& iV, double iEpsilon ) const
{
    return Math::isEqual(x(), iV.x(), iEpsilon) &&
        Math::isEqual(y(), iV.y(), iEpsilon) &&
        Math::isEqual(z(), iV.z(), iEpsilon);
}

//---------------------------------------------------------------------------
double Vector3::norm() const
{
    return std::sqrt(mData[0]*mData[0] + mData[1]*mData[1] + mData[2]*mData[2]);
}

//---------------------------------------------------------------------------
double Vector3::normSquare() const
{
    return mData[0]*mData[0] + mData[1]*mData[1] + mData[2]*mData[2];
}

//---------------------------------------------------------------------------
Vector3& Vector3::normalize()
{
    double n = norm();
    
    //avoid dividing by zéro...
    if( !isEqual(n, 0.0) )
    { (*this) /= this->norm(); }

    return (*this);
}

Vector3	Vector3::multiplyComponents(const Vector3& v) const
{
	return Vector3(this->x()*v.x(), this->y()*v.y(), this->z()*v.z());
}

//----------------------------------------------------------------------------
// strictement egale, pour une comparaison un peu plus permissive, voir
// Math::isEqual( const Vector3&, const Vector3&, double )
bool Vector3::operator== (const Vector3& iV) const
{
    return x() == iV.x() &&
        y() == iV.y() &&
        z() == iV.z();
}

//----------------------------------------------------------------------------
bool Vector3::operator!= ( const Vector3& iV ) const
{ return !( *this == iV ); }

//----------------------------------------------------------------------------
Vector3 Vector3::operator+ (const Vector3 &vect) const
{
    Vector3 r;

    r.mData[0] = x() + vect.x();
    r.mData[1] = y() + vect.y();
    r.mData[2] = z() + vect.z();

    return r;
}

//----------------------------------------------------------------------------
Vector3& Vector3::operator+= (const Vector3 &vect)
{
    mData[0] += vect.x();
    mData[1] += vect.y();
    mData[2] += vect.z();
    return *this;
}

//----------------------------------------------------------------------------
Vector3 Vector3::operator- (const Vector3 &vect) const
{
    Vector3 r;

    r.mData[0] = x() - vect.x();
    r.mData[1] = y() - vect.y();
    r.mData[2] = z() - vect.z();

    return r;
}

//----------------------------------------------------------------------------
Vector3 Vector3::operator- () const
{ 
    Vector3 r;
       
    r.mData[0] = -mData[0];
    r.mData[1] = -mData[1];
    r.mData[2] = -mData[2];

    return r;
}

//----------------------------------------------------------------------------
Vector3& Vector3::operator-= (const Vector3 &iVect)
{
    *this = *this - iVect;
    return *this;
}

//----------------------------------------------------------------------------
Vector3 Vector3::operator* (double val) const
{
    Vector3 r;

    r.mData[0] = x() * val;
    r.mData[1] = y() * val;
    r.mData[2] = z() * val;

    return r;
}

//----------------------------------------------------------------------------
Vector3& Vector3::operator*= (double val)
{
    mData[0] *= val;
    mData[1] *= val;
    mData[2] *= val;
    return *this;
}

//----------------------------------------------------------------------------
Vector3 Vector3::operator/ (double val) const
{
    double vTmp;
    Vector3 vect(*this);

    const double e = std::numeric_limits<double>::epsilon();
    if(val>e || val<-e)
      vTmp=(1.0)/val;
    else
      vTmp=0.0;
      
    vect *= vTmp;    
    return vect;
}

//----------------------------------------------------------------------------
Vector3& Vector3::operator/= (double val)
{
    double vTmp;

    const double e = std::numeric_limits<double>::epsilon();
    if(val>e || val<-e)
      vTmp=(1.0)/val;
    else
      vTmp=0.0;
      
        *this *= vTmp;
    return *this;
}
//----------------------------------------------------------------------------
// cross product
Vector3 Vector3::operator^ (const Vector3 &vect) const
{
    Vector3 r;

    r.mData[0] = y() * vect.z() - z() * vect.y();
    r.mData[1] = z() * vect.x() - x() * vect.z();
    r.mData[2] = x() * vect.y() - y() * vect.x();

    return r;
}

//----------------------------------------------------------------------------
// scalar product
double Vector3::operator* (const Vector3 &vect) const
{
    return ( x() * vect.x() + y() * vect.y() + z() * vect.z() );
}

//----------------------------------------------------------------------------
bool Vector3::operator> (const Vector3& iV) const
{
    return mData[0] > iV.mData[0] &&
        mData[1] > iV.mData[1] &&
        mData[2] > iV.mData[2];
}

//----------------------------------------------------------------------------
bool Vector3::operator>= (const Vector3& iV) const
{
    return mData[0] >= iV.mData[0] &&
        mData[1] >= iV.mData[1] &&
        mData[2] >= iV.mData[2];
}

//----------------------------------------------------------------------------
bool Vector3::operator< (const Vector3& iV) const
{
    return mData[0] < iV.mData[0] &&
        mData[1] < iV.mData[1] &&
        mData[2] < iV.mData[2];
}

//----------------------------------------------------------------------------
bool Vector3::operator<= (const Vector3& iV) const
{
    return mData[0] <= iV.mData[0] &&
        mData[1] <= iV.mData[1] &&
        mData[2] <= iV.mData[2];
}

//---------------------------------------------------------------------------
void Vector3::set(double iX, double iY, double iZ)
{ mData[0] = iX; mData[1] = iY; mData[2] = iZ; }

//---------------------------------------------------------------------------
void Vector3::setX(double iX)
{ mData[0] = iX; }

//---------------------------------------------------------------------------
void Vector3::setY(double iY)
{ mData[1] = iY; }

//---------------------------------------------------------------------------
void Vector3::setZ(double iZ)
{ mData[2] = iZ; }

//----------------------------------------------------------------------------
std::string Vector3::toString(int iPrecision /*=3*/) const
{ 
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(iPrecision);
  oss << "(" << x() << ", " << y() << ", " << z() << ")";
  return oss.str();
}

//---------------------------------------------------------------------------
double Vector3::x() const
{ return mData[0]; }

//---------------------------------------------------------------------------
double Vector3::y() const
{ return mData[1]; }

//---------------------------------------------------------------------------
double Vector3::z() const
{ return mData[2]; }

//---------------------------------------------------------------------------
Vector2 Vector3::xy() const
{ return Vector2(mData[0], mData[1]); }

//---------------------------------------------------------------------------
Vector2 Vector3::xz() const
{ return Vector2(mData[0], mData[2]); }

//---------------------------------------------------------------------------
Vector2 Vector3::yz() const
{ return Vector2(mData[1], mData[2]); }

//---------------------------------------------------------------------------
// operator for scalar x Vector2
//
Vector3 Realisim::Math::operator*(double iV, const Vector3& iVect)
{
    return iVect * iV;
}
