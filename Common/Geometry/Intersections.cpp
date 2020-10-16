

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
    //--- Plane
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
    //-------------------------------------------------------------------------
    IntersectionType intersect(const Line& iL,
                               const Plane& iP,
                               Math::Vector3 *oP,
                               Math::Vector3 *oNormal,
                               double *oD)
    {
        return intersectLinePlane(iL,
            iP.getNormal(),
            iP.getPoint(),
            oP, oNormal, oD);
    }

    //-------------------------------------------------------------------------
    IntersectionType intersectLinePlane(const Line& iL,
        const Vector3& iNormalOfPlane,
        const Vector3& iPosOnPlane,
        Math::Vector3 *oP,
        Math::Vector3 *oNormal,
        double *oD)
    {
        Vector3 p;
        IntersectionType t;

        const Vector3 l = iL.getDirection();
        const Vector3 &n = iNormalOfPlane;
        const Vector3 l0 = iL.getOrigin();
        const Vector3 p0 = iPosOnPlane;

        double d = 0.0;
        // check if parallel
        if (isEqual(l*n, 0.0, 1e-5))
        {
            // check if line is contained
            if (isEqual((p0 - l0)*n, 0.0, 1e-5))
            {
                t = itLine;
            }
            else
            {
                t = itNone;
            }
        }
        else
        {
            t = itPoint;
            // compute the point of intersection d
            d = (p0 - l0)*n / (l*n);
            p = l*d + l0;
        }

        if (oP != nullptr) { *oP = p; }
        if (oNormal != nullptr) { *oNormal = n; }
        if (oD != nullptr) { *oD = d; }
        return t;
    }

    //-------------------------------------------------------------------------
    //--- Sphere
    //-------------------------------------------------------------------------
    bool intersects(const Line& iL,
        const Sphere& iS, IntersectionType* ipIt/*= nullptr*/)
    {
        IntersectionType t = intersect(iL, iS, nullptr, nullptr);
        
        if(ipIt)
        { *ipIt = t; }
        return t != itNone;
    }
    
    //-------------------------------------------------------------------------
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

    //-------------------------------------------------------------------------
    //--- Triangle
    //-------------------------------------------------------------------------
    bool intersects(const Line& iL,
        const Triangle& iT, 
        IntersectionType* ipIt /*= nullptr*/)
    {
        IntersectionType iType = intersect(iL, iT, nullptr, nullptr, nullptr);

        if (ipIt) *ipIt = iType;

        return iType != itNone;
    }

    //-------------------------------------------------------------------------
    IntersectionType intersect(const Line& iL,
        const Triangle& iT,
        Vector3 *oP /*= nullptr*/,
        Vector3 *oNormal /*= nullptr*/,
        double *oD /*= nullptr*/)
    {
        IntersectionType iType = itNone;
        Vector3 p;
        Vector3 n;
        double d;

        // perform a planar intersection.
        const vector<Vector3> &vertices = iT.getVertices();
        iType = intersectLinePlane(iL, iT.getNormal(), vertices[0], &p, &n, &d);

        // line coplanar with plane is treated has not intersecting

        // validate the intersection point lies in the triangle
        //
        // perform a dot product from each segment of the triangle with point of
        // intersection.
        // if all result are positive, then the points lies in the polygon
        //
        if (iType == itPoint)
        {
            const Vector3 a = (vertices[1] - vertices[0]) ^ (p - vertices[0]);
            const Vector3 b = (vertices[2] - vertices[1]) ^ (p - vertices[1]);
            const Vector3 c = (vertices[0] - vertices[2]) ^ (p - vertices[2]);

            if( !(a*n >= 0 && b*n >= 0 && c*n >= 0) )
            {
                iType = itNone;
                p = Vector3();
                n = Vector3();
                d = 0.0;
            }
        }

        if (oP) *oP = p;
        if (oNormal) *oNormal = n;
        if (oD) *oD = d;
        return iType;
    }

    //-------------------------------------------------------------------------
    //--- Mesh
    //-------------------------------------------------------------------------
    bool intersects(const Line& iL, const Mesh& iM, IntersectionType* oType /*= nullptr*/)
    {
        IntersectionType t = intersect(iL, iM, nullptr, nullptr, nullptr);

        if (oType) *oType = t;
        return t != itNone;
    }

    //-------------------------------------------------------------------------
    IntersectionType intersect(const Line& iL, const Mesh& iM,
        std::vector<Math::Vector3> *oPoints /*= nullptr*/,
        std::vector<Math::Vector3> *oNormals /*= nullptr*/,
        std::vector<double> *oDs /*= nullptr*/)
    {
        // for all triangles...
        const vector<Mesh::Face> &faces = iM.getFaces();
        const vector<Mesh::VertexData> &vertices = iM.getVertices();
        const int numFaces = (int)faces.size();
        Triangle tri;
        IntersectionType iType = itNone;

        Vector3 p, n; //intersection point, normal
        double d; // distance from ray origin.
        int numIntersections = 0;
        for(int i = 0; i < numFaces; ++i)
        {
            tri.set(vertices[faces[i].mVertexIndices[0]].mVertex,
                vertices[faces[i].mVertexIndices[1]].mVertex,
                vertices[faces[i].mVertexIndices[2]].mVertex);

            iType = intersect(iL, tri, &p, &n, &d);

            if (iType == itPoint)
            {
                numIntersections++;
                if (oPoints) oPoints->push_back(p);
                if (oNormals) oNormals->push_back(n);
                if (oDs) oDs->push_back(d);
            }
        }

        switch (numIntersections)
        {
        case 0: iType = itNone; break;
        case 1: iType = itPoint; break;
        default: iType = itPoints; break;
        }

        return iType;
    }


    //-------------------------------------------------------------------------
    //--- AxisAlignedBoundingBox
    //-------------------------------------------------------------------------
    bool intersects(const Line& iL, const AxisAlignedBoundingBox& iAabb, IntersectionType* oType /*= nullptr*/)
    {
        const Vector3 &vmin = iAabb.getMinCorner();
        const Vector3 &vmax = iAabb.getMaxCorner();
        const Vector3 &rpos = iL.getOrigin();
        const Vector3 &rdir = iL.getDirection();
        const Vector3 rdir_inv( 1.0/rdir.x(), 1.0/rdir.y(), 1.0/rdir.z() );
        // shamelessly striped off:
        // https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
        //
        double tx1 = (vmin.x() - rpos.x())*rdir_inv.x();
        double tx2 = (vmax.x() - rpos.x())*rdir_inv.x();

        double tmin = min(tx1, tx2);
        double tmax = max(tx1, tx2);

        double ty1 = (vmin.y() - rpos.y())*rdir_inv.y();
        double ty2 = (vmax.y() - rpos.y())*rdir_inv.y();

        tmin = max(tmin, min(ty1, ty2));
        tmax = min(tmax, max(ty1, ty2));

        double tz1 = (vmin.z() - rpos.z())*rdir_inv.z();
        double tz2 = (vmax.z() - rpos.z())*rdir_inv.z();

        tmin = max(tmin, min(tz1, tz2));
        tmax = min(tmax, max(tz1, tz2));

        const bool r = tmax >= tmin;
        if (oType) *oType = r ? itPoint : itNone;
        return r;
    }

    //-------------------------------------------------------------------------
    IntersectionType intersect(const Line& iL, const AxisAlignedBoundingBox& iAabb,
        std::vector<Math::Vector3> *oPoints /*= nullptr*/,
        std::vector<Math::Vector3> *oNormals /*= nullptr*/,
        std::vector<double> *oDs /*= nullptr*/)
    {
        // THIS IS CRAPPY
        // LOOK ON THE INTERNET FOR A LINE - AABB INTERSECTION TEST
        //
        Mesh m = iAabb.makeMesh();
        IntersectionType iType = intersect(iL, m, oPoints, oNormals, oDs);
        return iType;
    }

    //-------------------------------------------------------------------------
    //--- RectangularPrism
    //-------------------------------------------------------------------------
    bool intersects(const Line& iL, const RectangularPrism& iB, IntersectionType* oType /*= nullptr*/)
    {
        AxisAlignedBoundingBox aabb;
        aabb.set(iB.getFarBottomLeft(), iB.getNearTopRight());
        return intersects(iL, aabb, oType);
    }

    //-------------------------------------------------------------------------
    IntersectionType intersect(const Line& iL, const RectangularPrism& iB,
        std::vector<Math::Vector3> *oPoints /*= nullptr*/,
        std::vector<Math::Vector3> *oNormals /*= nullptr*/,
        std::vector<double> *oDs /*= nullptr*/)
    {
        // THIS IS CRAPPY
        // LOOK ON THE INTERNET FOR A LINE - AABB INTERSECTION TEST
        //
        Mesh m = iB.makeMesh();
        IntersectionType iType = intersect(iL, m, oPoints, oNormals, oDs);

        return iType;
    }

    //-------------------------------------------------------------------------
    //--- OctreeOfMeshFaces
    //-------------------------------------------------------------------------
    bool intersects(const Line& iL, const OctreeOfMeshFaces& iO, IntersectionType* oType /*= nullptr*/)
    {
        IntersectionType t = intersect(iL, iO, nullptr, nullptr, nullptr);

        if (oType) *oType = t;
        return t != itNone;
    }

    //-------------------------------------------------------------------------
    IntersectionType intersect(const Line& iL, const OctreeOfMeshFaces& iO,
        std::vector<Math::Vector3> *oPoints /*= nullptr*/,
        std::vector<Math::Vector3> *oNormals /*= nullptr*/,
        std::vector<double> *oDs /*= nullptr*/)
    {
        const OctreeOfMeshFaces::Node *pRoot = iO.getRoot();
        
        // dig until we find a leaf child, then intersect with all
        // the triangles.
        intersect(iL, pRoot, iO.getMesh(), oPoints, oNormals, oDs);

        IntersectionType iType = itNone;
        if (oPoints)
        {
            if( oPoints->size() == 1 )
            { iType = itPoint; }
            if (oPoints->size() > 1) {
                iType = itPoints;
            }
            else {
                iType = itNone;
            }
        }

        return iType;
    }

    //-------------------------------------------------------------------------
    void intersect(const Line& iL, 
        const OctreeOfMeshFaces::Node *ipN,
        const Mesh *ipMesh,
        std::vector<Math::Vector3> *oPoints /*= nullptr*/,
        std::vector<Math::Vector3> *oNormals /*= nullptr*/,
        std::vector<double> *oDs /*= nullptr*/)
    {
        // dig if aabb intersects
        if (intersects(iL, ipN->mAabb))
        {
            // this is a leaf node, intersects with all triangles
            if (!ipN->hasChilds())
            {
                IntersectionType iType;
                Vector3 p;
                Vector3 n;
                double d;

                const int numFaces = (int)ipN->mTriangles.size();

                for (int i = 0; i < numFaces; ++i)
                {
                    iType = intersect(iL, ipN->mTriangles[i], &p, &n, &d);

                    if (iType == itPoint)
                    {
                        if (oPoints) oPoints->push_back(p);
                        if (oNormals) oNormals->push_back(n);
                        if (oDs) oDs->push_back(d);
                    }
                }
                return;
            }

            const int numberOfChilds = ipN->getNumberOfChilds();
            for (int i = 0; i < numberOfChilds; ++i)
            {
                OctreeOfMeshFaces::Node *c = ipN->mChilds[i];
                intersect(iL, c, ipMesh, oPoints, oNormals, oDs);
            }
        }
    }
}
}