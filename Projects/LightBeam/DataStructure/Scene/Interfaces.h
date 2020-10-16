#pragma once
#include "Geometry/AxisAlignedBoundingBox.h"
#include "DataStructure/Light.h"
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
        enum NodeType{ntSceneNode, ntRenderable, ntLight};
    
        ISceneNode();
        explicit ISceneNode(NodeType);
        ISceneNode(const ISceneNode&) = default;
        ISceneNode& operator=(const ISceneNode&) = default;
        virtual ~ISceneNode() = 0;

        const Geometry::AxisAlignedBoundingBox& getAxisAlignedBoundingBox() const;
        uint32_t getId() const;
        NodeType getNodeType() const;

    protected:
        void setAxisAlignedBoundingBox(const Geometry::AxisAlignedBoundingBox&);

        static uint32_t mIdCounter;
        uint32_t mId;
        NodeType mType;
        Geometry::AxisAlignedBoundingBox mAABB;
        //Matrix4 mParentTransform
        //Matrix4 mWorldTransform
    };

    //-------------------------------------------------------------------------
    class IRenderable
    {
    public:
        IRenderable();
        IRenderable(const IRenderable&) = default;
        IRenderable& operator=(const IRenderable&) = default;
        virtual ~IRenderable() = 0;

        virtual bool intersects(const Geometry::Line& iRay) const = 0;
        virtual bool intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const = 0;

        const std::shared_ptr<Material> getMaterial() const;
        void setMaterial(std::shared_ptr<Material>);
        
    protected:
        std::shared_ptr<Material> mpMaterial;
    };
    
    //-------------------------------------------------------------------------
    class ILightNode
    {
    public:
        ILightNode();
        ILightNode(const ILightNode&) = default;
        ILightNode& operator=(const ILightNode&) = default;
        virtual ~ILightNode() = 0;

        virtual double computeLi() const;
        const Light& getLight() const;
        void setLight(const Light&);
//        const std::shared_ptr<Core::Image> getCutout() const;
//        void setCutout(std::shared_ptr<Core::Image>);
        
    protected:
        //std::shared_ptr<Core::Image> mpCutout;
        Light mLight;
    };

}
}