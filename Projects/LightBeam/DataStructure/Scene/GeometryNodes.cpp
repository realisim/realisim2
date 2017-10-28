#include "GeometryNodes.h"
#include "Geometry/Intersections.h"
#include "IntersectionResult.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace LightBeam;
    using namespace Math;

//-------------------------------------------------------------------------
//--- Plane
//-------------------------------------------------------------------------
PlaneNode::PlaneNode() : ISceneNode(ntRenderable),
    IRenderable(),
    mPlane()
{
}

//-------------------------------------------------------------------------
const Plane& PlaneNode::getPlane() const
{ return mPlane; }

//-------------------------------------------------------------------------
bool PlaneNode::intersects(const Line& iRay) const
{
    return Geometry::intersects(iRay, mPlane);
}

//-------------------------------------------------------------------------
bool PlaneNode::intersect(const Line& iRay, IntersectionResult* opResult) const
{
    Vector3 p;
    Vector3 n;
    double d;
    Geometry::IntersectionType it = Geometry::intersect(iRay, mPlane, &p, &n, &d);
    
    if(opResult && it != itNone)
    {
        (*opResult).mNormal = n;
        (*opResult).mD = d;
        (*opResult).mpMaterial = getMaterial();
    }
    return it != itNone;
}

//-------------------------------------------------------------------------
void PlaneNode::setPlane(const Plane& iPlane)
{ mPlane = iPlane; }


//-------------------------------------------------------------------------
//--- Sphere
//-------------------------------------------------------------------------
SphereNode::SphereNode() : ISceneNode(ntRenderable),
    IRenderable(),
    mSphere()
{
}

//-------------------------------------------------------------------------
const Sphere& SphereNode::getSphere() const
{ return mSphere; }

//-------------------------------------------------------------------------
bool SphereNode::intersects(const Line& iRay) const
{ return Geometry::intersects(iRay, mSphere); }

//-------------------------------------------------------------------------
bool SphereNode::intersect(const Line& iRay, IntersectionResult* opResult) const
{
    std::vector<Vector3> p;
    std::vector<Vector3> n;
    std::vector<double> d;
    Geometry::IntersectionType it = Geometry::intersect(iRay, mSphere, &p, &n, &d);
    
    if(opResult && it == itPoints)
    {
        // We want the point closest to the origin of the
        // ray in the direction of the ray. This means the smallest d positive
        // d.
        //
        int index = 0;
        if(d[0] >=0 && d[1] >= 0)
        { index = d[0] < d[1] ? 0 : 1; }
        else if( d[0] < 0 )
        { index = 1; }
        else
        { index = 0 ;}
        
        (*opResult).mNormal = n[index];
        (*opResult).mD = d[index];
        (*opResult).mpMaterial = getMaterial();
    }
    return it != itNone;
}

//-------------------------------------------------------------------------
void SphereNode::setSphere(const Sphere& iS)
{ mSphere = iS; }