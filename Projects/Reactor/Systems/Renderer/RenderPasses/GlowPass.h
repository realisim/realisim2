#pragma once

#include "Systems/Renderer/IRenderable.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"

namespace Realisim
{
namespace Reactor
{
    class GlowPass : public IRenderPass
    {
    public:
        GlowPass();
        GlowPass(const GlowPass&) = delete;
        GlowPass& operator=(const GlowPass&) = delete;
        virtual ~GlowPass();

    protected:
        virtual void initializeFbo() override final;
        virtual void loadShader(const std::string& iAssetPath) override final;
        virtual void sharePasses(const std::map<int, IRenderPass*> ipRenderPassNameToRenderPass) final;
        virtual void defineInputOutputs() override final;
        virtual void connectInputOutputs() override final;
        virtual void applyGlState() final;
        virtual void render(const Rendering::Camera&, const std::vector<IRenderable*> iRenderables) final;
        virtual void revertGlState() final;

        void applyBlur();
        void renderGlowingModels(const Rendering::Camera&, const std::vector<IRenderable*> iRenderables);

        Rendering::Camera mFullScreen2dCam;
        Rendering::VertexArrayObject mFullScreenQuad;

        Rendering::Shader mDrawGlowTexturedModels;
        Rendering::Shader mBlurShader;

        Rendering::FrameBufferObject* mpOpaquePassFbo;
        Rendering::FrameBufferAttachementType mGlowTexturedModelFba;

        Output mOpaqueGlowTexturedOut;
    };

}
}