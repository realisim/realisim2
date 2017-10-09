
#pragma once
#include "Mesh.h"
#include "Plane.h"

namespace Realisim
{
namespace Geometry
{
    class Frustum
    {
    public:
        Frustum();
        Frustum(const Frustum&) = default;
        Frustum& operator= (const Frustum&) = default;
        ~Frustum() = default;

        // the enum is made so pnNear is the first item and pnFar the second...
        // so when iterating over all plane, we start with near and far first.
        //
        // It is meant that way for efficiency reasons, please do not change the
        // order.
        // 
        enum PlaneName{pnNear = 0, pnFar, pnLeft, pnRight, pnBottom, pnTop, pnCount};

        bool contains(const Math::Vector3& iPoint, bool iProper = false) const;
        const Math::Vector3* getCorners() const;
        Plane getPlane(int) const;
        Mesh makeMesh() const;
        void setPlane(PlaneName iName, const Plane& iPlane);
        void set(Math::Vector3 corners[8]);

    protected:
        bool isPointAbovePlane(const Math::Vector3& iP, PlaneName iName, bool iProper) const;

        Plane mPlanes[pnCount];
        Math::Vector3 mCorners[8];
    };

}
}