
#pragma once

#include "3d/Material.h"
#include "DataStructure/Scene/MaterialNode.h"
#include <memory>

namespace Realisim { namespace Geometry { class Line; } }
namespace Realisim { namespace LightBeam { class IntersectionResult; } }

namespace Realisim
{
namespace LightBeam
{

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

        const std::shared_ptr<MaterialNode> getMaterialNode() const;
        void setMaterialNode(std::shared_ptr<MaterialNode>);
        
    protected:

        std::shared_ptr<MaterialNode> mpMaterialNode;
    };
    
}
}