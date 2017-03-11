
#include "3d/Projection.h"
#include <cmath>

using namespace Realisim;
using namespace Math;
using namespace TreeD;

//-----------------------------------------------------------------------------
Projection::Projection() :
mLeft(-1),
mRight(1),
mBottom(-1),
mTop(1),
mNear(1),
mFar(10),
mZoomFactor(1.0),
mType(tOrthogonal)
{}

//-----------------------------------------------------------------------------
double Projection::bottom() const
{ return mBottom; }

//-----------------------------------------------------------------------------
double Projection::far() const
{ return mFar; }

//-----------------------------------------------------------------------------
double Projection::left() const
{ return mLeft; }

//-----------------------------------------------------------------------------
double Projection::height() const
{ return fabs(mTop - mBottom); }

//-----------------------------------------------------------------------------
double Projection::near() const
{ return mNear; }

//-----------------------------------------------------------------------------
double Projection::right() const
{ return mRight; }

//-----------------------------------------------------------------------------
void Projection::setBottom(double iV)
{ mBottom = iV; }

//-----------------------------------------------------------------------------
void Projection::setFar(double iV)
{ mFar = iV; }

//-----------------------------------------------------------------------------
void Projection::setLeft(double iV)
{ mLeft = iV; }

//-----------------------------------------------------------------------------
void Projection::setNear(double iV)
{ mNear = iV; }

//-----------------------------------------------------------------------------
void Projection::setOrthoProjection(double iVisibleGlUnit, double iNear, double iFar)
{
    setProjection(-iVisibleGlUnit / 2.0, iVisibleGlUnit / 2.0,
                  -1.0, 1.0, iNear, iFar, Projection::tOrthogonal);
}

//-----------------------------------------------------------------------------
void Projection::setOrthoProjection(double iVisibleGlUnitX, double iVisibleGlUnitY,
                                    double iNear, double iFar)
{
    setProjection(-iVisibleGlUnitX / 2.0, iVisibleGlUnitX / 2.0,
                  -iVisibleGlUnitY / 2.0, iVisibleGlUnitY / 2.0, iNear, iFar,
                  Projection::tOrthogonal);
}

//-----------------------------------------------------------------------------
/*iFov est en degree, iRatio est généralement le ratio du viewport sous la
 forme height / width ) */
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
    mZoomFactor = 1.0;
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

//-----------------------------------------------------------------------------
double Projection::zoom() const
{ return mZoomFactor; }
