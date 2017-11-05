#pragma once

#include "Material.h"
#include "Math/Vector.h"
#include <memory>

namespace Realisim
{
namespace LightBeam
{
    // members:
    // mNormal: normal of the surface to the point of intersection
    //  it is always normalized.
    //  mD: d is the parameric distance from the origin of the ray where the
    //      intersection occured
    //
    // mW0: unit ray from intersectionPoint to camera.
    // mWi: unit vector from intersectionPoint to Light.
    //
    class IntersectionResult
    {
    public:
        IntersectionResult();
        IntersectionResult(const IntersectionResult&) = default;
        IntersectionResult& operator=(const IntersectionResult&) = default;
        ~IntersectionResult() = default;

        Math::Vector3 mNormal;
        double mD; //parametric distance along the ray
        Math::Vector3 mW0;
        Math::Vector3 mWi;
        std::shared_ptr<Material> mpMaterial;
    };

}
}