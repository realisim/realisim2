
#include <cmath>
#include <iomanip>
#include "Math/IsEqual.h"
#include "Math/Vector.h"
#include <sstream>

using namespace Realisim;
    using namespace Math;

Vector4::Vector4()
{ mData[0] = mData[1] = mData[2] = mData[3] = 0.0; }

//-----------------------------------------------------------------------------
Vector4::Vector4(double x, double y, double z, double w)
{ set(x, y, z, w); }

//-----------------------------------------------------------------------------
Vector4::Vector4(const Vector2& v2, double z, double w)
{
    mData[0] = v2.x();
    mData[1] = v2.y();
    mData[2] = z;
    mData[3] = w;
}

//-----------------------------------------------------------------------------
Vector4::Vector4(const Vector3& v3, double w)
{
    mData[0] = v3.x();
    mData[1] = v3.y();
    mData[2] = v3.z();
    mData[3] = w;
}

//-----------------------------------------------------------------------------
Vector4::Vector4(double v)
{
    mData[0] = mData[1] = mData[2] = mData[3] = v;
}

//---------------------------------------------------------------------------
double Vector4::norm() const
{
    return std::sqrt(mData[0]*mData[0] + mData[1]*mData[1] + mData[2]*mData[2] + mData[3]*mData[3]);
}

//---------------------------------------------------------------------------
Vector4& Vector4::normalize()
{
    double n = norm();
    
    //avoid dividing by zéro...
    if( !isEqual(n, 0.0) )
    { (*this) /= this->norm(); }
    
    return (*this);

}

Vector4	Vector4::multiplyComponents(const Vector4& v) const
{
	return Vector4(this->x()*v.x(), this->y()*v.y(), this->z()*v.z(), this->w()*v.w());
}

//---------------------------------------------------------------------------
const double* Vector4::dataPointer() const
{
    return &mData[0];
}

//---------------------------------------------------------------------------
bool Vector4::isEqual(const Vector4& iV, double iEpsilon ) const
{
    return Math::isEqual(x(), iV.x(), iEpsilon) &&
        Math::isEqual(y(), iV.y(), iEpsilon) &&
        Math::isEqual(z(), iV.z(), iEpsilon) &&
        Math::isEqual(w(), iV.w(), iEpsilon);
}

//-----------------------------------------------------------------------------
Vector4 Vector4::operator+(const Vector4& iV)
{
    return Vector4(mData[0] + iV.mData[0],
        mData[1] + iV.mData[1],
        mData[2] + iV.mData[2],
        mData[3] + iV.mData[3]);
}

//-----------------------------------------------------------------------------
Vector4& Vector4::operator+=(const Vector4& iV)
{
    mData[0] = mData[0] + iV.mData[0];
    mData[1] = mData[1] + iV.mData[1];
    mData[2] = mData[2] + iV.mData[2];
    mData[3] = mData[3] + iV.mData[3];
    return *this;
}


//-----------------------------------------------------------------------------
Vector4 Vector4::operator-(const Vector4& iV)
{
    return Vector4(mData[0] - iV.mData[0],
        mData[1] - iV.mData[1],
        mData[2] - iV.mData[2],
        mData[3] - iV.mData[3]);
}

//-----------------------------------------------------------------------------
Vector4 Vector4::operator- () const
{
    return Vector4( -x(), -y(), -z(), -w() );
}

//-----------------------------------------------------------------------------
Vector4& Vector4::operator-=(const Vector4& iV)
{
    *this = *this - iV;
    return *this;
}

//-----------------------------------------------------------------------------
Vector4 Vector4::operator* (double iV) const
{
    Vector4 r;

    r.mData[0] = x() * iV;
    r.mData[1] = y() * iV;
    r.mData[2] = z() * iV;
    r.mData[3] = w() * iV;

    return r;
}

//-----------------------------------------------------------------------------
Vector4& Vector4::operator*= (double iV)
{
    *this = *this * iV;
    return *this;
}

//-----------------------------------------------------------------------------
Vector4 Vector4::operator/ (double iV) const
{
    return *this * (1.0/iV);
}

//-----------------------------------------------------------------------------
Vector4& Vector4::operator/= (double iV)
{
    *this = *this / iV;
    return *this;
}

//-----------------------------------------------------------------------------
bool Vector4::operator== ( const Vector4& iV) const
{
    return x() == iV.x() &&
        y() == iV.y() &&
        z() == iV.z() &&
        w() == iV.w();
}

//-----------------------------------------------------------------------------
bool Vector4::operator!= ( const Vector4& iV ) const
{
    return !(*this == iV);
}

//-----------------------------------------------------------------------------
double Vector4::operator* (const Vector4 &vect) const // dot product
{
    return ( x() * vect.x() + 
        y() * vect.y() + 
        z() * vect.z() +
        w() * vect.w() );
}

//----------------------------------------------------------------------------
bool Vector4::operator> (const Vector4& iV) const
{
    return mData[0] > iV.mData[0] &&
        mData[1] > iV.mData[1] &&
        mData[2] > iV.mData[2] &&
        mData[3] > iV.mData[3];
}

//----------------------------------------------------------------------------
bool Vector4::operator>= (const Vector4& iV) const
{
    return mData[0] >= iV.mData[0] &&
        mData[1] >= iV.mData[1] &&
        mData[2] >= iV.mData[2] &&
        mData[3] >= iV.mData[3];
}

//----------------------------------------------------------------------------
bool Vector4::operator< (const Vector4& iV) const
{
    return mData[0] < iV.mData[0] &&
        mData[1] < iV.mData[1] &&
        mData[2] < iV.mData[2] &&
        mData[3] < iV.mData[3];
}

//----------------------------------------------------------------------------
bool Vector4::operator<= (const Vector4& iV) const
{
    return mData[0] <= iV.mData[0] &&
        mData[1] <= iV.mData[1] &&
        mData[2] <= iV.mData[2] &&
        mData[3] <= iV.mData[3];
}

//-----------------------------------------------------------------------------
void Vector4::set(double x, double y, double z, double w)
{
    mData[0] = x;
    mData[1] = y;
    mData[2] = z;
    mData[3] = w;
}

//-----------------------------------------------------------------------------
void Vector4::setX(double x)
{ mData[0] = x; }

//-----------------------------------------------------------------------------
void Vector4::setY(double y)
{ mData[1] = y; }

//-----------------------------------------------------------------------------
void Vector4::setZ(double z)
{ mData[2] = z; }

//-----------------------------------------------------------------------------
void Vector4::setW(double w)
{ mData[3] = w; }

//-----------------------------------------------------------------------------
std::string Vector4::toString(int iPrecision /*=3*/) const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(iPrecision);
    oss << "(" << x() << ", " << y() << ", " << z() << ", " << w() << ")";
    return oss.str();
}

//-----------------------------------------------------------------------------
double Vector4::x() const
{ return mData[0]; }

//-----------------------------------------------------------------------------
double Vector4::y() const
{ return mData[1]; }

//-----------------------------------------------------------------------------
double Vector4::z() const
{ return mData[2]; }

//-----------------------------------------------------------------------------
double Vector4::w() const
{ return mData[3]; }

//-----------------------------------------------------------------------------
Vector3 Vector4::xyz() const
{ return Vector3(mData[0], mData[1], mData[2]); }

//---------------------------------------------------------------------------
// operator for scalar x Vector2
//
Vector4 Realisim::Math::operator*(double iV, const Vector4& iVect)
{
    return iVect * iV;
}