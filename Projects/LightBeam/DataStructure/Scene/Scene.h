#pragma once

#include "Interfaces.h"
#include <memory>
#include <vector>

namespace Realisim
{
namespace LightBeam
{
    class IntersectionResult;
    
    class Scene// : IRenderable
    {
    public:
        Scene();
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        ~Scene();

        void addNode(std::shared_ptr<ISceneNode>);
        const std::vector< std::shared_ptr<ILightNode> >& getLights() const;
        const std::vector< std::shared_ptr<IRenderable> >& getRenderables() const;

    protected:
        std::vector< std::shared_ptr<ISceneNode> > mNodes;
        std::vector< std::shared_ptr<IRenderable> > mRenderables;
        std::vector< std::shared_ptr<ILightNode> > mLights;
    };
}
}