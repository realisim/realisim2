#pragma once

#include "AxisAlignedBoundingBox.h"
#include "Line.h"
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
    enum IntersectionType{itNone, itPoint, itPoints, itLine, itPlane};

    //--- Plane
    bool intersects(const Line&, const Plane&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Plane&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr, double *oD = nullptr);
    IntersectionType intersectLinePlane(const Line& iL, const Math::Vector3& iNormalOfPlane, const Math::Vector3& iPosOnPlane, Math::Vector3 *oP, Math::Vector3 *oNormal, double *oD);

    ////--- Plane
    //bool intersects(const Plane&, const Line&, IntersectionType* = nullptr);
    //IntersectionType intersect(const Plane&, const Line&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr);

    //--- Sphere
    bool intersects(const Line&, const Sphere&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Sphere&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- triangle
    bool intersects(const Line&, const Triangle&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Triangle&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr, double *oD = nullptr);

    //--- Mesh
    bool intersects(const Line&, const Mesh&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Mesh&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- AxisAlignedBoundingBox
    bool intersects(const Line&, const AxisAlignedBoundingBox&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const AxisAlignedBoundingBox&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- RectangularPrism
    bool intersects(const Line&, const RectangularPrism&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const RectangularPrism&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

    //--- OctreeOfMeshFaces
    bool intersects(const Line&, const OctreeOfMeshFaces&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const OctreeOfMeshFaces&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);
    void intersect(const Line&, const OctreeOfMeshFaces::Node*, const Mesh *, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr, std::vector<double> *oDs = nullptr);

}
}