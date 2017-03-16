#define _USE_MATH_DEFINES

#include "Rendering/Projection.h"
#include <cmath>

using namespace Realisim;
using namespace Math;
using namespace Rendering;

//-----------------------------------------------------------------------------
Projection::Projection() :
mLeft(-1),
mRight(1),
mBottom(-1),
mTop(1),
mNear(1),
mFar(10),
mType(tOrthogonal)
{}

//-----------------------------------------------------------------------------
Projection::Projection(double iLeft, double iRight,
           double iBottom, double iTop,
           double iNear, double iFar, Projection::Type iType) :
mLeft(iLeft),
mRight(iRight),
mBottom(iBottom),
mTop(iTop),
mNear(iNear),
mFar(iFar),
mType(iType)
{}

//-----------------------------------------------------------------------------
double Projection::bottom() const
{ return mBottom; }

//-----------------------------------------------------------------------------
double Projection::farPlane() const
{ return mFar; }

//-----------------------------------------------------------------------------
double Projection::left() const
{ return mLeft; }

//-----------------------------------------------------------------------------
double Projection::height() const
{ return fabs(mTop - mBottom); }

//-----------------------------------------------------------------------------
double Projection::nearPlane() const
{ return mNear; }

//-----------------------------------------------------------------------------
Math::Matrix4 Projection::projectionMatrix() const
{
    Matrix4 result;

    const double l = left();
    const double r = right();
    const double b = bottom();
    const double t = top();
    const double f = farPlane();
    const double n = nearPlane();

    switch (type())
    {
    case Projection::tOrthogonal:
    {
        double m[4][4] = {
            { 2.0 / (r - l), 0.0, 0.0, -(r + l) / (r - l) },
            { 0.0, 2.0 / (t - b), 0.0, -(t + b) / (t - b) },
            { 0.0, 0.0, -2.0 / (f - n), -(f + n) / (f - n) },
            { 0, 0, 0, 1.0 },
        };
        result.set(m[0], true);
    }    break;
    case Projection::tPerspective:
    {
        double m[4][4] = {
            { (2.0*n) / (r - l), 0.0, (r + l) / (r - l), 0.0 },
            { 0.0, (2.0*n) / (t - b), (t + b) / (t - b), 0.0 },
            { 0, 0, -(f + n) / (f - n), (-2 * f * n) / (f - n) },
            { 0.0, 0.0, -1, 0.0 },
        };
        result.set(m[0], true);
    } break;
    default: break;
    }

    return result;
}

//-----------------------------------------------------------------------------
double Projection::right() const
{ return mRight; }

//-----------------------------------------------------------------------------
void Projection::setBottom(double iV)
{ mBottom = iV; }

//-----------------------------------------------------------------------------
void Projection::setFarPlane(double iV)
{ mFar = iV; }

//-----------------------------------------------------------------------------
void Projection::setLeft(double iV)
{ mLeft = iV; }

//-----------------------------------------------------------------------------
void Projection::setNearPlane(double iV)
{ mNear = iV; }

//-----------------------------------------------------------------------------
// iFov est en degree, iRatio est généralement le ratio du viewport sous la
// forme width / height ).
//
void Projection::setPerspectiveProjection(double iFov, double iRatio,
                                          double iNear, double iFar )
{
    const double kDegreeToRadian = M_PI/180.0;
    double halfHeight = iNear * tan(iFov * 0.5 * kDegreeToRadian);

    double halfWidth = iRatio * halfHeight;
    setProjection(-halfWidth, halfWidth, -halfHeight, halfHeight,
                  iNear, iFar, Projection::tPerspective );
}

//-----------------------------------------------------------------------------
void Projection::setProjection(double iLeft, double iRight,
                               double iBottom, double iTop,
                               double iNear, double iFar,
                               Projection::Type iType)
{
    mLeft = iLeft;
    mRight = iRight;
    mBottom = iBottom;
    mTop = iTop;
    mNear = iNear;
    mFar = iFar;
    mType = iType;
}

//-----------------------------------------------------------------------------
void Projection::setRight(double iV)
{ mRight = iV; }

//-----------------------------------------------------------------------------
void Projection::setTop(double iV)
{ mTop = iV; }

//-----------------------------------------------------------------------------
Math::Vector2 Projection::size() const
{ return Vector2(width(), height()); }

//-----------------------------------------------------------------------------
double Projection::top() const
{ return mTop; }

//-----------------------------------------------------------------------------
Projection::Type Projection::type() const
{ return mType; }

//-----------------------------------------------------------------------------
double Projection::width() const
{ return fabs(mRight - mLeft); }

