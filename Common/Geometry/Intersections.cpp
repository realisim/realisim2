

#include <cmath>
#include "Intersections.h"
#include "Math/IsEqual.h"
#include <vector>

namespace Realisim
{
namespace Geometry
{
    using namespace Math;
    using namespace std;

    //-------------------------------------------------------------------------
    //--- Line
    //-------------------------------------------------------------------------
    //--- line/plane
    bool intersects(const Line& iL, const Plane& iP, IntersectionType* ipIt)
    {
        IntersectionType t = intersect(iL, iP, nullptr, nullptr);

        if(ipIt != nullptr){ *ipIt = t; }

        return t != itNone;
    }

    //--- line/plane
    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    //
    IntersectionType intersect(const Line& iL,
                               const Plane& iP,
                               Math::Vector3 *oP,
                               Math::Vector3 *oNormal,
                               double *oD)
    {
        Vector3 p;
        IntersectionType t;
  
        const Vector3 l = iL.getDirection();
        const Vector3 n = iP.getNormal();
        const Vector3 l0 = iL.getOrigin();
        const Vector3 p0 = iP.getPoint();

        double d = 0.0;
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
            d = (p0-l0)*n / (l*n);
            p = l*d + l0;
        }

        if(oP != nullptr) { *oP = p; }
        if(oNormal != nullptr) { *oNormal = iP.getNormal(); }
        if(oD != nullptr) { *oD = d; }
        return t;
    }

    //-------------------------------------------------------------------------
    //--- Plane
    //-------------------------------------------------------------------------
    // plane/Line
    bool intersects(const Plane& iP, const Line& iL, IntersectionType* ipIt)
    { return intersects(iL, iP, ipIt); }

    // plane/Line
    IntersectionType intersect(const Plane& iP,
                               const Line& iL,
                               Math::Vector3* oP,
                               Math::Vector3* oNormal)
    { return intersect(iL, iP, oP, oNormal); }

    //-------------------------------------------------------------------------
    //--- Sphere
    //-------------------------------------------------------------------------
    bool intersects(const Line& iL,
        const Sphere& iS, IntersectionType* ipIt/*= nullptr*/)
    {
        bool r = false;
        IntersectionType t = intersect(iL, iS, nullptr, nullptr);
        
        if(ipIt)
        { *ipIt = t; }
        return r;
    }
    
    // return the contact(s) point(s) and normalize normal for
    // line/sphere intersection.
    //
    // https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    //
    IntersectionType intersect(const Line& iL,
        const Sphere& iS,
        vector<Vector3> *oPoints,
        vector<Vector3> *oNormals,
        vector<double> *oDs)
    {
        IntersectionType result = itNone;
        vector<Vector3> points = { Vector3(), Vector3() };
        vector<Vector3> normals = { Vector3(), Vector3() };
        vector<double> ds = { 0.0, 0.0 };
        
        const Vector3 o = iL.getOrigin();
        const Vector3 l = iL.getDirection();
        const Vector3 c = iS.getCenter();
        const double r = iS.getRadius();
        
        const Vector3 oMinusC = o - c;
        const double t0 = l*oMinusC;
        const double oMinusCNorm = oMinusC.norm();
        const double sqrtValue = (t0*t0) - oMinusCNorm*oMinusCNorm + r*r;
        
        if( sqrtValue < 0 )
        {
            result = itNone;
        }
        else if ( isEqual(sqrtValue, 1e-5) )
        {
            result = itPoint;
        }
        else
        {
            const double d0 = -t0 - sqrt(sqrtValue);
            const double d1 = -t0 + sqrt(sqrtValue);
            
            points[0] = o + d0*l;
            points[1] = o + d1*l;
            
            normals[0] = (points[0] - c).normalize();
            normals[1] = (points[1] - c).normalize();
            
            ds[0] = d0;
            ds[1] = d1;
            result = itPoints;
        }
        
        if(oPoints) { *oPoints = points; }
        if(oNormals) { *oNormals = normals; }
        if(oDs) { *oDs = ds; }
        
        return result;
    }

}
}