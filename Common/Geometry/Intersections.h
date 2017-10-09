#pragma once

#include "Line.h"
#include "Math/Vector.h"
#include "Plane.h"

namespace Realisim
{
namespace Geometry
{
    enum IntersectionType{itNone, itPoint, itLine, itPlane};

    //--- Line
    bool intersects(const Line&, const Plane&, IntersectionType* = nullptr);
    IntersectionType intersect(const Line&, const Plane&, Math::Vector3 *oP, Math::Vector3 *oNormal);

    //--- Plane
    bool intersects(const Plane&, const Line&, IntersectionType* = nullptr);
    IntersectionType intersect(const Plane&, const Line&, Math::Vector3 *oP, Math::Vector3 *oNormal);

}
}