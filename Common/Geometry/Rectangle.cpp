#include <algorithm>
#include <cassert>
#include "Rectangle.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;

//------------------------------------------------------------------------------
Rectangle::Rectangle() :
    mIsValid(false),
    mBottomLeft(),
    mTopRight()
{}

//------------------------------------------------------------------------------
Rectangle::Rectangle(const Vector2& iBottomLeft,
    const Vector2& iTopRight) :
    mBottomLeft(iBottomLeft),
    mTopRight(iTopRight)
{
    validate(); 
}

//------------------------------------------------------------------------------
Rectangle::Rectangle(const Vector2& iBottomLeft, 
    double iWidth,
    double iHeight) :
    mBottomLeft(iBottomLeft),
    mTopRight(mBottomLeft + Vector2(iWidth, iHeight))
{
    validate();
}

//------------------------------------------------------------------------------
Rectangle::~Rectangle()
{}

//------------------------------------------------------------------------------
// returns true if the point iPos is inside or on the edge of the rectangle.
// otherwise, returns false.
//
// If proper is true, it returns true only if the point is inside the rectangle.
//
bool Rectangle::contains( const Vector2& iPos, bool iProper /*=false*/) const
{
    bool r = false;
    if (iProper)
    {
        r = iPos.x() > getLeft() &&
            iPos.x() < getRight() &&
            iPos.y() > getBottom() &&
            iPos.y() < getTop();
    }
    else
    {
        r = iPos.x() >= getLeft() &&
            iPos.x() <= getRight() &&
            iPos.y() >= getBottom() &&
            iPos.y() <= getTop();
    }
    return r;
}

//------------------------------------------------------------------------------
// returns true if the rectangle iRect is inside or on the edge of the
// current rectangle.
//
// If proper is true, it returns true only if the rectangle is completely inside 
// the rectangle (not touching the edge)
bool Rectangle::contains(const Rectangle& iRect, bool iProper /*= false*/) const
{
    return contains(iRect.getBottomLeft(), iProper) &&
        contains(iRect.getTopRight(), iProper);
}

//------------------------------------------------------------------------------
double Rectangle::getArea() const
{
    const Vector2 s = getSize();
    return s.x() * s.y();
}

//------------------------------------------------------------------------------
Vector2 Rectangle::getCenter() const
{
    return mBottomLeft + getSize() / 2.0;
}

//------------------------------------------------------------------------------
double Rectangle::getBottom() const
{ return mBottomLeft.y(); }

//------------------------------------------------------------------------------
Vector2 Rectangle::getBottomLeft() const
{ return mBottomLeft; }

//------------------------------------------------------------------------------
Vector2 Rectangle::getBottomRight() const
{ return Vector2(mTopRight.x(), mBottomLeft.y()); }

//------------------------------------------------------------------------------
double Rectangle::getHeight() const
{ return mTopRight.y() - mBottomLeft.y(); }

//------------------------------------------------------------------------------
double Rectangle::getLeft() const
{ return mBottomLeft.x(); }

//------------------------------------------------------------------------------
// Returns the point iIndex, obviously, iIndex cannot be greater than 3 since
// a rectangle has 4 points...
//
// The points are given in counter clockwise order starting from bottomLeft.
//
Math::Vector2 Rectangle::getPoint(int iIndex) const
{
    Vector2 r;
    assert(iIndex <= 3);
    switch (iIndex)
    {
    case 0: r = getBottomLeft(); break;
    case 1: r = getBottomRight(); break;
    case 2: r = getTopLeft(); break;
    case 3: r = getTopRight(); break;
    default: break;
    }
    return r;
}

//------------------------------------------------------------------------------
double Rectangle::getRight() const
{ return mTopRight.x(); }

//------------------------------------------------------------------------------
Vector2 Rectangle::getSize() const
{ return mTopRight - mBottomLeft; }

//------------------------------------------------------------------------------
double Rectangle::getTop() const
{ return mTopRight.y(); }

//------------------------------------------------------------------------------
Vector2 Rectangle::getTopLeft() const
{ return Vector2(mBottomLeft.x(), mTopRight.y()); }

//------------------------------------------------------------------------------
Vector2 Rectangle::getTopRight() const
{ return mTopRight; }

//------------------------------------------------------------------------------
double Rectangle::getWidth() const
{ return mTopRight.x() - mBottomLeft.x(); }

//------------------------------------------------------------------------------
bool Rectangle::intersects(const Rectangle& iR) const
{
    bool r = false;
    if (isValid() && iR.isValid())
    {
        // we are using the minkowski method to determine if the rectangles
        // are intersecting.    

        Vector2 min(std::numeric_limits<double>::max());
        Vector2 max(-std::numeric_limits<double>::max());
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
            {
                Vector2 p = iR.getPoint(j) - getPoint(i);
                min.setX(std::min(min.x(), p.x()));
                min.setY(std::min(min.y(), p.y()));
                max.setX(std::max(max.x(), p.x()));
                max.setY(std::max(max.y(), p.y()));
            }

        Rectangle mink(min, max);
        r = mink.contains(Vector2(0.0), true);
    }
    
    return r;
}

//------------------------------------------------------------------------------
Rectangle Rectangle::intersected(const Rectangle& iR) const
{
    // We are the AABB overlap method to determine the resulting rectangle from
    // the intersection.
    Rectangle r;

    if (intersects(iR))
    {
        Vector2 bottomLeft(std::max(getLeft(), iR.getLeft()),
            std::max(getBottom(), iR.getBottom()));

        Vector2 topRight(std::min(getRight(), iR.getRight()),
            std::min(getTop(), iR.getTop()));
        r.set(bottomLeft, topRight);
    }

    return r;
}

//------------------------------------------------------------------------------
bool Rectangle::isValid() const
{
    return mIsValid;
}

//------------------------------------------------------------------------------
void Rectangle::set(const Vector2& iBottomLeft, const Vector2& iTopRight)
{
    mBottomLeft = iBottomLeft;
    mTopRight = iTopRight;
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::set(const Vector2& iBottomLeft, double iWidth, double iHeight)
{ set(iBottomLeft, iBottomLeft + Vector2(iWidth, iHeight)); }

//------------------------------------------------------------------------------
void Rectangle::setBottom(double iB)
{ 
    mBottomLeft.setY(iB);
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::setBottomLeft(const Vector2& iBottomLeft)
{
    mBottomLeft = iBottomLeft;
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::setBottomRight(const Vector2& iBottomRight)
{
    mBottomLeft.setY(iBottomRight.y());
    mTopRight.setX(iBottomRight.x());
    validate();
}

//------------------------------------------------------------------------------
// Modifies the height of the rectangle, the modification is applied to the
// top edge, the bottom edge remains unchanged.
//
void Rectangle::setHeight(double iV)
{
    mTopRight.setY(mBottomLeft.y() + iV);
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::setLeft(double iV)
{
    mBottomLeft.setX(iV);
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::setRight(double iV)
{
    mTopRight.setX(iV);
    validate();
}

//------------------------------------------------------------------------------
// Modifies the size of the rectangle by changing the top and right edges. The
// bottom and left edges remains unchanged
//
void Rectangle::setSize( const Vector2& iS)
{
    mTopRight = mBottomLeft + iS;
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::setTop(double iV)
{
    mTopRight.setY(iV);
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::setTopLeft(const Vector2& iTopLeft)
{
    mBottomLeft.setX(iTopLeft.x());
    mTopRight.setY(iTopLeft.y());
    validate();
}

//------------------------------------------------------------------------------
void Rectangle::setTopRight(const Vector2& iTopRight)
{
    mTopRight = iTopRight;
    validate();
}

//------------------------------------------------------------------------------
// Modifies the width of the rectangle. Only the right edge is modified.
// left edge remains unmodified.
//
void Rectangle::setWidth(double iWidth)
{
    mTopRight.setX(mBottomLeft.x() + iWidth);
    validate();
}

//------------------------------------------------------------------------------
Rectangle Rectangle::united(const Rectangle& iR) const
{
    return iR;
}

//------------------------------------------------------------------------------
void Rectangle::validate()
{
    mIsValid = mTopRight >= mBottomLeft;
}
