#pragma once

#include "Line.h"
#include "Math/Vector.h"
#include "Plane.h"
#include "Sphere.h"
#include <vector>

namespace Realisim
{
namespace Geometry
{
    enum IntersectionType{itNone, itPoint, itPoints, itLine, itPlane};

    //--- Line
    bool intersects(const Line&, const Plane&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Plane&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr);

    //--- Plane
    bool intersects(const Plane&, const Line&, IntersectionType* = nullptr);
    IntersectionType intersect(const Plane&, const Line&, Math::Vector3 *oP = nullptr, Math::Vector3 *oNormal = nullptr);

    //--- Sphere
    bool intersects(const Line&, const Sphere&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Sphere&, std::vector<Math::Vector3> *oPoints = nullptr, std::vector<Math::Vector3> *oNormals = nullptr);
}
}