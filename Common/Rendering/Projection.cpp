#include <cmath>
#include "Rendering/Projection.h"


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
Projection::Projection(double iFovX, double iRatio, double iNear, double iFar)
{
	setPerspectiveProjectionWithRatio(iFovX, iRatio, iNear, iFar);
}

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
double Projection::getBottom() const
{ return mBottom; }

//-----------------------------------------------------------------------------
double Projection::getFar() const
{ return mFar; }

//-----------------------------------------------------------------------------
double Projection::getHeight() const
{
	return fabs(mTop - mBottom);
}

//-----------------------------------------------------------------------------
double Projection::getLeft() const
{ return mLeft; }

//-----------------------------------------------------------------------------
double Projection::getNear() const
{ return mNear; }

//-----------------------------------------------------------------------------
Math::Matrix4 Projection::getProjectionMatrix() const
{
    Matrix4 result;

    const double l = getLeft();
    const double r = getRight();
    const double b = getBottom();
    const double t = getTop();
    const double f = getFar();
    const double n = getNear();

    switch (getType())
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
double Projection::getRight() const
{ return mRight; }

//-----------------------------------------------------------------------------
Math::Vector2 Projection::getSize() const
{ return Vector2(getWidth(), getHeight()); }

//-----------------------------------------------------------------------------
double Projection::getTop() const
{ return mTop; }

//-----------------------------------------------------------------------------
Projection::Type Projection::getType() const
{ return mType; }

//-----------------------------------------------------------------------------
double Projection::getWidth() const
{ return fabs(mRight - mLeft); }

//-----------------------------------------------------------------------------
bool Projection::operator==(const Projection& iP)
{
	return mLeft == iP.mLeft &&
		mRight == iP.mRight &&
		mBottom == iP.mBottom &&
		mTop == iP.mTop &&
		mNear == iP.mNear &&
		mFar == iP.mFar &&
		mType == iP.mType;
}

//-----------------------------------------------------------------------------
bool Projection::operator!=(const Projection& iP)
{
	return !operator==(iP); 
}

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
void Projection::setPerspectiveProjection(double iFovX, double iFovY, double iNear, double iFar)
{
	const double kDegreeToRadian = M_PI / 180.0;
	const double halfWidth = iNear * tan(iFovX * 0.5 * kDegreeToRadian);
	const double halfHeight = iNear * tan(iFovY * 0.5 * kDegreeToRadian);

	setProjection(-halfWidth, halfWidth, -halfHeight, halfHeight,
		iNear, iFar, Projection::tPerspective);
}

//-----------------------------------------------------------------------------
// iFov est en degree, iRatio est généralement le ratio du viewport sous la
// forme width / height ).
//
void Projection::setPerspectiveProjectionWithRatio(double iFovX, double iRatio,
                                          double iNear, double iFar )
{
    const double kDegreeToRadian = M_PI/180.0;
	const double halfWidth = iNear * tan(iFovX * 0.5 * kDegreeToRadian);
	const double halfHeight = halfWidth/iRatio;

    setProjection(-halfWidth, halfWidth, -halfHeight, halfHeight,
                  iNear, iFar, Projection::tPerspective );
}

//-----------------------------------------------------------------------------
void Projection::setRight(double iV)
{ mRight = iV; }

//-----------------------------------------------------------------------------
void Projection::setTop(double iV)
{ mTop = iV; }

