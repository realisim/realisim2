#pragma once

namespace Realisim
{
namespace Reactor {

    class IRenderable
    {
    public:
        IRenderable() = default;
        IRenderable(ThreeD::SceneNode* ipNode) : mpNode(ipNode) { ; }
        IRenderable(const IRenderable&) = delete;
        IRenderable& operator=(const IRenderable&) = delete;
        virtual ~IRenderable() = 0 { ; }

        virtual void initializeGpuRessources() = 0 { ; }
        virtual void draw() = 0 { ; }
        ThreeD::SceneNode* getSceneNode() { return mpNode; }
        virtual void releaseGpuRessources() = 0 { ; }

    protected:
        ThreeD::SceneNode* mpNode; //not owned, shall never be null
    };

}
}

