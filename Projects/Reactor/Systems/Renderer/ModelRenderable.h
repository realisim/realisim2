#pragma once

#include "Rendering/Gpu/Texture2d.h"
#include "Rendering/Gpu/VertexArrayObject.h"
#include "Systems/Renderer/IRenderable.h"

namespace Realisim
{
namespace Reactor {

    class ModelNode;

    class ModelRenderable : public IRenderable
    {
    public:
        ModelRenderable() = delete;
        explicit ModelRenderable(ModelNode* ipNode);
        ModelRenderable(const ModelRenderable&) = delete;
        ModelRenderable& operator=(const ModelRenderable&) = delete;
        virtual ~ModelRenderable();

        virtual void initializeGpuRessources() override;
        virtual void draw() override;
        ModelNode* getModelNode() { return mpNode; }
        const Rendering::Texture2d* getTexture() const;
        virtual void releaseGpuRessources() override;
        void setTexture(const Rendering::Texture2d* ipTexture);

    protected:
        ModelNode* mpNode; //not owned, shall never be null

        std::vector<Rendering::VertexArrayObject*> mVaoPtrs;
        const Rendering::Texture2d* mpTexture; // not owned
    };

}
}

