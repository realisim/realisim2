#pragma once

#include "AxisAlignedBoundingBox.h"
#include "Line.h"
#include "LineSegment.h"
#include "Math/Vector.h"
#include "OctreeOfMeshFaces.h"
#include "Plane.h"
#include "RectangularPrism.h"
#include "Sphere.h"
#include "Triangle.h"
#include <vector>

namespace Realisim
{
namespace Geometry
{
    enum IntersectionType{itNone, itPoint, itPoints, itLine, itLineSegment, itPlane, itLineLoop, itFullyContained};

    //--- Line - Plane
    bool intersects(const Line&, const Plane&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Plane&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr, double *oD = nullptr);
    IntersectionType intersectLinePlane(const Line& iL, const Math::Vector3& iNormalOfPlane, const Math::Vector3& iPosOnPlane, Math::Vector3 *oP, Math::Vector3 *oNormal, double *oD);

    ////--- Plane
    //bool intersects(const Plane&, const Line&, IntersectionType* = nullptr);
    //IntersectionType intersect(const Plane&, const Line&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr);

    //--- Line - Sphere
    bool intersects(const Line&, const Sphere&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Sphere&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- line - triangle
    bool intersects(const Line&, const Triangle&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Triangle&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr, double *oD = nullptr);

    //--- line - Mesh
    bool intersects(const Line&, const Mesh&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Mesh&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- line - AxisAlignedBoundingBox
    bool intersects(const Line&, const AxisAlignedBoundingBox&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const AxisAlignedBoundingBox&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- line - RectangularPrism
    bool intersects(const Line&, const RectangularPrism&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const RectangularPrism&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- line - OctreeOfMeshFaces
    bool intersects(const Line&, const OctreeOfMeshFaces&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const OctreeOfMeshFaces&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);
    void intersect(const Line&, const OctreeOfMeshFaces::Node*, const Mesh *, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //------ triangle - plane
    bool intersects(const Triangle&, const Plane&, IntersectionType* = nullptr);
    IntersectionType intersect(const Triangle&, const Plane&, std::vector<Math::Vector3> *oPoints = nullptr);
    //------ triangle - aabb
    bool intersects(const Triangle&, const AxisAlignedBoundingBox&, IntersectionType* = nullptr);
    bool intersect(const Triangle&, const AxisAlignedBoundingBox&, IntersectionType* = nullptr);

    //--- LineSegment - plane
    bool intersects(const LineSegment&, const Plane&, IntersectionType* = nullptr);
    IntersectionType intersect(const LineSegment&, const Plane&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr, double *oD = nullptr);

    //--- LineSegment - AABB
    bool intersects(const LineSegment&, const AxisAlignedBoundingBox&, IntersectionType* = nullptr);
    IntersectionType intersect(const LineSegment&, const AxisAlignedBoundingBox&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

}
}