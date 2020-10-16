#pragma once
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Mesh.h"

namespace Realisim
{
namespace Geometry
{
    class AxisAlignedBoundingBox
    {
    public:
        AxisAlignedBoundingBox();
        AxisAlignedBoundingBox(const AxisAlignedBoundingBox&) = default;
        AxisAlignedBoundingBox& operator= (const AxisAlignedBoundingBox&) = default;
        virtual ~AxisAlignedBoundingBox() = default;

		void add(const AxisAlignedBoundingBox &iOther);
        void addPoint(const Math::Vector3& iP);
        bool contains(const Math::Vector3& iPoint, bool iProper = false) const;
        double getArea() const;
        Math::Vector3 getCenter() const;
        std::vector<Math::Vector3> getCorners() const;
        const Math::Vector3& getMaxCorner() const;
        const Math::Vector3& getMinCorner() const;
        double getVolume() const;
        Math::Vector3 getSize() const;
        bool isValid() const;
        bool intersects(const AxisAlignedBoundingBox&) const; 
        Mesh makeMesh() const;
        void reset();
        void set(Math::Vector3 iMin, Math::Vector3 iMax);
        AxisAlignedBoundingBox& transform(const Math::Matrix4& iM);
        AxisAlignedBoundingBox transformed(const Math::Matrix4& iM) const;

        const bool operator == (const AxisAlignedBoundingBox&) const;
        const bool operator != (const AxisAlignedBoundingBox&) const;
        const bool operator <= (const AxisAlignedBoundingBox& aabb) const;
        const bool operator < (const AxisAlignedBoundingBox& aabb) const;

    protected:

        Math::Vector3 mMinCorner;
        Math::Vector3 mMaxCorner;
    };

}
}