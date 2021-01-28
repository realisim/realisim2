#pragma once

#include "Geometry/AxisAlignedBoundingBox.h"
#include "DataStructure/Light.h"
#include <memory>
#include <stdint.h>


namespace Realisim
{
namespace LightBeam
{
    
    //-------------------------------------------------------------------------
    class ISceneNode
    {
    public:
        enum NodeType{ntSceneNode, ntRenderable, ntLight, ntMaterial};
    
        ISceneNode();
        explicit ISceneNode(NodeType);
        ISceneNode(const ISceneNode&) = default;
        ISceneNode& operator=(const ISceneNode&) = default;
        virtual ~ISceneNode() = 0;

        const Geometry::AxisAlignedBoundingBox& getAxisAlignedBoundingBox() const;
        uint32_t getId() const;
        std::string getName() const;
        NodeType getNodeType() const;
        void setName(const std::string&);

    protected:
        void setAxisAlignedBoundingBox(const Geometry::AxisAlignedBoundingBox&);

        static uint32_t mIdCounter;
        uint32_t mId;
        NodeType mType;
        Geometry::AxisAlignedBoundingBox mAABB;
        std::string mName;
        //Matrix4 mParentTransform
        //Matrix4 mWorldTransform
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