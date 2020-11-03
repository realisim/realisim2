
#pragma once

#include "Vector.h"
#include "VectorI.h"

namespace Realisim
{
namespace Math
{

    inline Vector2 toVector2(const Vector2i &iV)
    { return Vector2((double)iV.x(), (double)iV.y()); }

    inline Vector2i toVector2i(const Vector2 &iV)
    { return Vector2i((int)iV.x(), (int)iV.y()); }
    
    inline Vector3 toVector3(const Vector3i &iV)
    { return Vector3((double)iV.x(), (double)iV.y(), (double)iV.z()); }

    inline Vector3i toVector3i(const Vector3 &iV)
    { return Vector3i((int)iV.x(), (int)iV.y(), (int)iV.z()); }

}
}