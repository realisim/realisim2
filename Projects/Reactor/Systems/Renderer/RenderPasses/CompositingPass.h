#pragma once

#include "Systems/Renderer/IRenderable.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"

namespace Realisim
{
namespace Reactor
{
    class CompositingPass : public IRenderPass
    {
    public:
        CompositingPass();
        CompositingPass(const CompositingPass&) = delete;
        CompositingPass& operator=(const CompositingPass&) = delete;
        virtual ~CompositingPass();

    protected:
        virtual void loadShader(const std::string& iAssetPath) final;
        virtual void sharePasses(const std::map<int, IRenderPass*>& ipRenderPassNameToRenderPass) final;
        virtual void defineInputOutputs() override final;
        virtual void releaseGlRessources() override final;
        virtual void applyGlState() final;
        virtual void render(const Rendering::Camera&, const std::vector<IRenderable*> iRenderables) final;
        virtual void revertGlState() final;

        Rendering::Camera mFullScreen2dCam;
        Rendering::VertexArrayObject mFullScreenQuad;

        Rendering::FrameBufferAttachementType mAdditionResultFba;
    };

}
}