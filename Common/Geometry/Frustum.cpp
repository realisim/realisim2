
#include <algorithm>
#include <cassert>
#include "Frustum.h"


using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

//-----------------------------------------------------------------------------
Frustum::Frustum()
{}

//-----------------------------------------------------------------------------
bool Frustum::contains(const Vector3& iPoint, bool iProper /*= false*/) const
{
    // a point is contained if it lies inside all planes

    bool isInside = true;
    for (int i = pnLeft; i < pnCount && isInside; ++i)
    {
        isInside &= isPointAbovePlane(iPoint, (PlaneName)i, iProper);
    }
    return isInside;
}

//-----------------------------------------------------------------------------
const Vector3* Frustum::getCorners() const
{
    return mCorners;
}

//-----------------------------------------------------------------------------
Plane Frustum::getPlane(int iName) const
{
    assert(iName >= pnNear && iName < pnCount);
    return mPlanes[iName];
}

//-----------------------------------------------------------------------------
bool Frustum::isPointAbovePlane(const Vector3& iP, PlaneName iName, bool iProper) const
{
    const Plane& p = mPlanes[iName];
    double dist = p.distance(iP);

    //now lets project v on n, if it is greater than zero, then the point is above the plane
    if(iProper)
    { return dist > 0.0; }
    else
    { return dist >= 0.0; }
}

//-----------------------------------------------------------------------------
Mesh Frustum::makeMesh() const
{
    Mesh mesh;

    return mesh;
}

//-----------------------------------------------------------------------------
void Frustum::setPlane(PlaneName iName, const Plane& iPlane)
{
    assert(iName >= pnNear && iName < pnCount);
    mPlanes[iName] = iPlane;
}

//-----------------------------------------------------------------------------
void Frustum::set(Vector3 corners[8])
{
    for (int i = 0; i<8; ++i)
        mCorners[i] = corners[i];

    this->setPlane(Realisim::Geometry::Frustum::pnLeft, Realisim::Geometry::Plane(mCorners[0], mCorners[4], mCorners[7]));
    this->setPlane(Realisim::Geometry::Frustum::pnRight, Realisim::Geometry::Plane(mCorners[1], mCorners[2], mCorners[5]));
    this->setPlane(Realisim::Geometry::Frustum::pnBottom, Realisim::Geometry::Plane(mCorners[1], mCorners[5], mCorners[0]));
    this->setPlane(Realisim::Geometry::Frustum::pnTop, Realisim::Geometry::Plane(mCorners[3], mCorners[7], mCorners[6]));
    this->setPlane(Realisim::Geometry::Frustum::pnNear, Realisim::Geometry::Plane(mCorners[0], mCorners[3], mCorners[2]));
    this->setPlane(Realisim::Geometry::Frustum::pnFar, Realisim::Geometry::Plane(mCorners[6], mCorners[7], mCorners[4]));
}
