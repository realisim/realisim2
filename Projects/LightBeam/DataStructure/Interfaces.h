#pragma once
#include "Material.h"
#include <memory>
#include <stdint.h>

namespace Realisim{ namespace Geometry {class Line; } }

namespace Realisim
{
namespace LightBeam
{
    class IntersectionResult;
    
    //-------------------------------------------------------------------------
    class ISceneNode
    {
    public:
        enum NodeType{ntSceneNode, ntRenderable, ntLights};
    
        ISceneNode();
        explicit ISceneNode(NodeType);
        ISceneNode(const ISceneNode&) = default;
        ISceneNode& operator=(const ISceneNode&) = default;
        virtual ~ISceneNode() = 0;

        uint32_t getId() const;
        NodeType getNodeType() const;

    protected:
        static uint32_t mIdCounter;
        uint32_t mId;
        NodeType mType;
    };

    //-------------------------------------------------------------------------
    class IRenderable
    {
    public:
        IRenderable();
        IRenderable(const IRenderable&) = default;
        IRenderable& operator=(const IRenderable&) = default;
        virtual ~IRenderable() = 0;

#pragma message("all intersects methods with line should also return the d: distance to origin. As it would be used as a distance to camera...")
        virtual bool intersects(const Geometry::Line& iRay) const = 0;
        virtual bool intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const = 0;

        const std::shared_ptr<Material> getMaterial() const;
        void setMaterial(std::shared_ptr<Material>);
        
    protected:
        //boundingbox
        //Matrix4 mParentTransform
        //Matrix4 mWorldTransform
        std::shared_ptr<Material> mpMaterial;
    };

}
}