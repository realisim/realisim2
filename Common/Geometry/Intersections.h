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
    Math::Vector3 intersect(const Line&, const Plane&, IntersectionType* = nullptr);

    //--- Plane
    bool intersects(const Plane&, const Line&, IntersectionType* = nullptr);
    Math::Vector3 intersect(const Plane&, const Line&, IntersectionType* = nullptr);

}
}