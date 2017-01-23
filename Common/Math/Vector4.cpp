
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
bool Vector4::isEqual(const Vector4& iV) const
{
    return Math::isEqual(x(), iV.x()) &&
        Math::isEqual(y(), iV.y()) &&
        Math::isEqual(z(), iV.z()) &&
        Math::isEqual(w(), iV.w());
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
Vector4& Vector4::operator-=(const Vector4& iV)
{
    mData[0] = mData[0] - iV.mData[0];
    mData[1] = mData[1] - iV.mData[1];
    mData[2] = mData[2] - iV.mData[2];
    mData[3] = mData[3] - iV.mData[3];
    return *this;
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