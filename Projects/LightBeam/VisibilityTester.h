#pragma once

#include "Material.h"
#include "Math/Vector.h"

namespace Realisim
{
namespace LightBeam
{
    class Scene;
    
    class VisibilityTester
    {
    public:
        VisibilityTester();
        VisibilityTester(const VisibilityTester&) = default;
        VisibilityTester& operator=(const VisibilityTester&) = default;
        ~VisibilityTester() = default;

        void set(const Math::Vector3& iPoint, const Math::Vector3& iLightPos, const Scene *);
        bool isOccluded();

    protected:
        const Scene *mpScene; //not owned
        Math::Vector3 mShadedPoint;
        Math::Vector3 mLightPosition;
    };

}
}