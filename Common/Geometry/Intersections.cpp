
#include "Core/Unused.h"
#include <cmath>
#include "Geometry/Utilities.h"
#include "Geometry/Intersections.h"
#include "Math/IsEqual.h"
#include <vector>

namespace Realisim
{
    namespace Geometry
    {
        using namespace Math;
        using namespace std;

        //-------------------------------------------------------------------------
        //--- line - Plane
        //-------------------------------------------------------------------------
        //--- line/plane
        bool intersects(const Line& iL, const Plane& iP, IntersectionType* ipIt)
        {
            IntersectionType t = intersect(iL, iP, nullptr, nullptr);

            if (ipIt != nullptr) { *ipIt = t; }

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
            if (isEqual(l*n, 0.0, 1e-8))
            {
                // check if line is contained
                if (isEqual((p0 - l0)*n, 0.0, 1e-8))
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
        //--- line - Sphere
        //-------------------------------------------------------------------------
        bool intersects(const Line& iL,
            const Sphere& iS, IntersectionType* ipIt/*= nullptr*/)
        {
            IntersectionType t = intersect(iL, iS, nullptr, nullptr);

            if (ipIt)
            {
                *ipIt = t;
            }
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

            if (sqrtValue < 0)
            {
                result = itNone;
            }
            else if (isEqual(sqrtValue, 1e-5))
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

            if (oPoints) { *oPoints = points; }
            if (oNormals) { *oNormals = normals; }
            if (oDs) { *oDs = ds; }

            return result;
        }

        //-------------------------------------------------------------------------
        //--- line - Triangle
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
            if (iType == itPoint &&
                !iT.contains(p))
            {
                iType = itNone;
            }

            if (oP) *oP = p;
            if (oNormal) *oNormal = n;
            if (oD) *oD = d;
            return iType;
        }

        //-------------------------------------------------------------------------
        //--- line - Mesh
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
            for (int i = 0; i < numFaces; ++i)
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
        //--- line - AxisAlignedBoundingBox
        //-------------------------------------------------------------------------
        bool intersects(const Line& iL, const AxisAlignedBoundingBox& iAabb, IntersectionType* oType /*= nullptr*/)
        {
            const Vector3 &vmin = iAabb.getMinCorner();
            const Vector3 &vmax = iAabb.getMaxCorner();
            const Vector3 &rpos = iL.getOrigin();
            const Vector3 &rdir = iL.getDirection();
            const Vector3 rdir_inv(1.0 / rdir.x(), 1.0 / rdir.y(), 1.0 / rdir.z());
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
        //--- line - RectangularPrism
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
        //--- line - OctreeOfMeshFaces
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
                if (oPoints->size() == 1)
                {
                    iType = itPoint;
                }
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


                    ////!!!!! TEMP CODE - INTERSECTION AABB TO DEBUG
                    //IntersectionType iType;
                    //std::vector<Vector3> p;
                    //std::vector<Vector3> n;
                    //std::vector<double> d;
                    //iType = intersect(iL, ipN->mAabb, &p, &n, &d);

                    //if (iType != itNone)
                    //{
                    //    if (oPoints) oPoints->insert(oPoints->end(), p.begin(), p.end());
                    //    if (oNormals) oNormals->insert(oNormals->end(), n.begin(), n.end());
                    //    if (oDs) oDs->insert(oDs->end(), d.begin(), d.end());
                    //}
                    ////!!!!! END OF TEMP CODE

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

        //-------------------------------------------------------------------------
        // triangle - plane
        //-------------------------------------------------------------------------
        bool intersects(const Triangle& iTri, const Plane& iPlane, IntersectionType *oType /*= nullptr*/)
        {
            if (oType) *oType = itNone;

            const Vector3& pNormal = iPlane.getNormal();
            const Vector3 vOnPlane = Geometry::getPerpendicularVector(pNormal);
            // if all vertices are on the same side of the plane, then
            // no intersection.
            enum status { sUndefined, sOnNormalSide, sNotOnNormalSide };
            status s = sUndefined;
            const std::vector<Vector3> &vertices = iTri.getVertices();
            for (int i = 0; i < 3; i++)
            {
                double projectionOnNormal = ((vertices[i] - iPlane.getPoint())) * pNormal;
                bool onNormalSide = projectionOnNormal > 0;

                // coplanar...
                // lets say they do are coPlanar... not sure what to do with that...
                if (isEqual(projectionOnNormal, 0.0, 1e-5))
                {
                    if (oType) *oType = itFullyContained;
                    return true;
                }

                switch (s)
                {
                case sUndefined: s = onNormalSide ? sOnNormalSide : sNotOnNormalSide; break;
                case sOnNormalSide:
                    if (!onNormalSide) {
                        if (oType) *oType = itLineSegment;
                        return true;
                    }
                    break;
                case sNotOnNormalSide:
                    if (onNormalSide) {
                        if (oType) *oType = itLineSegment;
                        return true;
                    }
                    break;
                }
            }

            return false;
        }

        //-------------------------------------------------------------------------
        IntersectionType intersect(const Triangle &iTri, const Plane &iPlane,
            std::vector<Math::Vector3> *oPoints /*= nullptr*/)
        {
            //if a triangle intersects a plane, it will result in a lineSegment (2 points)
            //The 2 points are defined by the intersection of the triangle edge and the plane.
            //
            IntersectionType iType = itNone;
            intersects(iTri, iPlane, &iType);
            if (iType == itLineSegment)
            {
                const std::vector<Vector3> &vertices = iTri.getVertices();
                Vector3 p, n;
                double d;
                Line l;

                l.set(vertices[0], vertices[1]);
                if (intersect(l, iPlane, &p, &n, &d) == itPoint) {
                    if (oPoints) oPoints->push_back(p);
                }

                l.set(vertices[1], vertices[2]);
                if (intersect(l, iPlane, &p, &n, &d) == itPoint) {
                    if (oPoints) oPoints->push_back(p);
                }

                l.set(vertices[2], vertices[0]);
                if (intersect(l, iPlane, &p, &n, &d) == itPoint) {
                    if (oPoints) oPoints->push_back(p);
                }
            }

            return iType;
        }

        //-------------------------------------------------------------------------
        bool intersects(const Triangle& iTri,
            const AxisAlignedBoundingBox& iAabb,
            IntersectionType* opType /*= nullptr*/)
        {
            // 2 cases
            // 1-   the triangle is somehow inside or has at lease one vertex
            //      inside.
            //      1.1 all vertices are contained -> itFullyContained
            //      1.2 not all vertices are contained -> perform a lineSegment/plane
            //          intersection with the aabb
            //
            // 2-   none of the triangle vertices are inside the box. 
            //      2.1 In that case we perform a lineSegment-plane intersection for each
            //          edge of the aabb
            //
            //      2.2 Perform an lineSegment-AABB intersection with all edges of triangle
            //      
            IntersectionType iType = itNone;

            bool allInside = true;
            bool allOutside = true;
            bool contained;
            for (auto &v : iTri.getVertices())
            {
                contained = iAabb.contains(v);
                allInside &= contained;
                allOutside &= !contained;
            }
            if (allInside)
            {
                iType = itFullyContained;
            }
            else if (allOutside)
            {
                AxisAlignedBoundingBox triAabb;
                const vector<Vector3> &triVs = iTri.getVertices();
                triAabb.addPoint(triVs[0]);
                triAabb.addPoint(triVs[1]);
                triAabb.addPoint(triVs[2]);

                if (triAabb.intersects(iAabb))
                {
                    iType = itPoint;
                }
                //// it might ou might not intersect
                ////
                //// make 12 segments and intersect with the plane
                //LineSegment segments[12];
                //std::vector<Vector3> c = iAabb.getCorners();
                //segments[0] = LineSegment(c[0], c[1]); // -Z
                //segments[1] = LineSegment(c[1], c[2]);
                //segments[2] = LineSegment(c[2], c[3]);
                //segments[3] = LineSegment(c[3], c[0]);
                //segments[4] = LineSegment(c[0], c[4]); // side
                //segments[5] = LineSegment(c[1], c[5]);
                //segments[6] = LineSegment(c[2], c[6]);
                //segments[7] = LineSegment(c[3], c[7]);
                //segments[8] = LineSegment(c[4], c[5]); // Z
                //segments[9] = LineSegment(c[5], c[6]);
                //segments[10] = LineSegment(c[6], c[7]);
                //segments[11] = LineSegment(c[7], c[4]);

                //Vector3 p, n;
                //double d;
                //Plane trianglePlane;
                //trianglePlane.set(iTri.getVertex(0), iTri.getNormal());
                //for (int i = 0; i < 12 && iType == itNone; ++i)
                //{
                //    if (intersect(segments[i], trianglePlane, &p, &n, &d))
                //    {
                //        // Now we have to validate the intersection point lies
                //        // on the triangle, else no intersection
                //        if (iTri.contains(p))
                //        {
                //            iType = itLineLoop;
                //        }
                //    }
                //}

                //// lineSegment AABB intersection
                //if(iType == itNone)
                //{
                //    const vector<Vector3> &v = iTri.getVertices();
                //    segments[0] = LineSegment(v[0], v[1]);
                //    segments[1] = LineSegment(v[1], v[2]);
                //    segments[2] = LineSegment(v[2], v[0]);
                //    
                //    for (int i = 0; i < 3 && iType == itNone; ++i)
                //    {
                //        intersects(segments[i], iAabb, &iType);
                //    }
                //}
            }
            else // somehow one or two vertices are inside.
            {
                iType = itLineSegment;
            }

            if (opType) *opType = iType;
            return iType != itNone;
        }

        //-------------------------------------------------------------------------
        //--- LineSegment - plane
        //-------------------------------------------------------------------------
        bool intersects(const LineSegment& iL, const Plane &iP, IntersectionType *oType /*= nullptr*/)
        {
            IntersectionType iType = intersect(iL, iP, nullptr, nullptr, nullptr);
            if (oType) *oType = iType;
            return iType != itNone;
        }

        //-------------------------------------------------------------------------
        IntersectionType intersect(const LineSegment &iL, const Plane &iP,
            Math::Vector3 *oP /*= nullptr*/,
            Math::Vector3 *oNormal /*= nullptr*/,
            double *oD /*= nullptr*/)
        {
            Line l;
            l.set(iL.mP0, iL.mP1);

            Vector3 p, n;
            double d;
            IntersectionType it = intersect(l, iP, &p, &n, &d);
            if (it == itPoint)
            {
                if (!iL.isPointOnSegment(p))
                {
                    it = itNone;
                }
            }

            if (oP) *oP = p;
            if (oNormal) *oNormal = n;
            if (oD) *oD = d;
            return it;
        }

        //-------------------------------------------------------------------------
        //--- LineSegment - AABB
        //-------------------------------------------------------------------------
        bool intersects(const LineSegment& iL, const AxisAlignedBoundingBox& iAabb,
            IntersectionType *oType /* = nullptr*/)
        {
            vector<Vector3> p, n;
            vector<double> d;
            IntersectionType iType = itNone;

            Line l(iL.mP0, iL.mP1);
            iType = intersect(l, iAabb, &p, &n, &d);
            if (iType != itNone)
            {
                iType = itNone;
                // validate that at least one point lies on the segment...
                for (int i = 0; i < (int)p.size() && iType == itNone; ++i)
                {
                    if (iL.isPointOnSegment(p[i]))
                    {
                        iType = itPoint;
                    }
                }
            }

            if (oType) *oType = iType;
            return iType != itNone;
        }

        //-------------------------------------------------------------------------
        IntersectionType intersect(const LineSegment& iL, const AxisAlignedBoundingBox& iAabb,
            vector<Math::Vector3> *oPoints /*= nullptr*/,
            vector<Math::Vector3> *oNormals /*= nullptr*/,
            vector<double> *oDs /*= nullptr*/)
        {
            UNUSED(iL);
            UNUSED(iAabb);
            UNUSED(oPoints);
            UNUSED(oNormals);
            UNUSED(oDs);

            return itNone;
        }

}
}