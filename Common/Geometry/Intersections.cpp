

#include "Intersections.h"
#include "Math/IsEqual.h"

namespace Realisim
{
namespace Geometry
{
    using namespace Math;

    //-------------------------------------------------------------------------
    //--- Line
    //-------------------------------------------------------------------------
    //--- line/plane
    bool intersects(const Line& iL, const Plane& iP, IntersectionType* ipIt)
    {
        IntersectionType t;
        intersect(iL, iP, &t);

        if(ipIt != nullptr){ *ipIt = t; }

        return t != itNone;
    }

    //--- line/plane
    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    //
    Math::Vector3 intersect(const Line& iL, const Plane& iP, IntersectionType* ipIt)
    {
        Vector3 r;
        IntersectionType t;
  
        const Vector3 l = iL.getDirection();
        const Vector3 n = iP.getNormal();
        const Vector3 l0 = iL.getOrigin();
        const Vector3 p0 = iP.getPoint();

        // check if parallel
        if( isEqual(l*n, 0.0, 1e-5) )
        {
            // check if line is contained
            if( isEqual( (p0-l0)*n , 0.0, 1e-5 ) )
            { t = itLine; }
            else
            { t = itNone; }
        }
        else
        {
            t = itPoint;
            // compute the point of intersection d
            const double d = (p0-l0)*n / (l*n);
            r = l*d + l0;
        }

        if(ipIt != nullptr) { *ipIt = t; }
        return r;
    }

    //-------------------------------------------------------------------------
    //--- Plane
    //-------------------------------------------------------------------------
    // plane/Line
    bool intersects(const Plane& iP, const Line& iL, IntersectionType* ipIt)
    { return intersects(iL, iP, ipIt); }

    // plane/Line
    Math::Vector3 intersect(const Plane& iP, const Line& iL, IntersectionType* ipIt)
    { return intersect(iL, iP, ipIt); }

}
}