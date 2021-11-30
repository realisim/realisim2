#pragma once

#include "Geometry/AxisAlignedBoundingBox.h"
#include "Math/Matrix.h"

namespace Realisim
{
namespace ThreeD
{

    class IPositionableNode
    {
    public:
        IPositionableNode();
        IPositionableNode(const IPositionableNode&) = default;
        IPositionableNode& operator=(const IPositionableNode&) = default;
        virtual ~IPositionableNode() = 0;

        const Geometry::AxisAlignedBoundingBox& getWorldSpaceAABB() const { return mUpdatedWorldSpaceAABB; }
        const Math::Matrix4& getParentTransform() const { return mParentTransform; }
        const Math::Matrix4& getWorldTransform() const { return mWorldTransform; }

        virtual void initializeModelSpaceAABB() = 0;
        bool isTransformDirty() const { return mIsTransformDirty; }

        void setParentTransform(const Math::Matrix4& iV);
        void setTransformDirty(bool iV) { mIsTransformDirty = iV; }
        void setWorldTransform(const Math::Matrix4& iV);

        virtual void updateWorldSpaceAABB();

    protected:
        bool mIsTransformDirty;

        Math::Matrix4 mParentTransform; // transform relative to parent
        Math::Matrix4 mWorldTransform; // transform relative to world origin

        Geometry::AxisAlignedBoundingBox mOriginalModelSpaceAABB;
        Geometry::AxisAlignedBoundingBox mUpdatedWorldSpaceAABB;
    };

}
}

