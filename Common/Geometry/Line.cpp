
#include "Math/IsEqual.h"
#include "Line.h"

using namespace Realisim;
    using namespace Geometry;

//-----------------------------------------------------------------------------
Line::Line() :
    mOrigin(),
    mDirection()
{}

//-----------------------------------------------------------------------------
Line::Line(const Math::Vector3& p0, const Math::Vector3& p1) :
    mOrigin(p0),
    mDirection(p1 - p0)
{
    mDirection.normalize();
}

//-----------------------------------------------------------------------------
// returns the direction as a unit vector.
//
const Math::Vector3& Line::getDirection() const
{ return mDirection; }

//-----------------------------------------------------------------------------
const Math::Vector3& Line::getOrigin() const
{ return mOrigin; }

//-----------------------------------------------------------------------------
bool Line::isValid() const
{
    return mDirection.isEqual( Math::Vector3(0.0) );
}

//-----------------------------------------------------------------------------
void Line::set(const Math::Vector3& p0, const Math::Vector3& p1)
{
    mOrigin = p0;
    mDirection = p1 - p0;
    mDirection.normalize();
}

//-----------------------------------------------------------------------------
void Line::setDirection(const Math::Vector3& iD)
{ mDirection = iD; }

//-----------------------------------------------------------------------------
void Line::setOrigin(const Math::Vector3& p0)
{ mOrigin = p0; }
